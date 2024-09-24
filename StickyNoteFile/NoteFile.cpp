#include "NoteFile.h"
#include <iostream>
#include <objbase.h>
#include <ctime>
#include <QDateTime>
#include "version.hpp"

using namespace pugi;
NF::NoteFile::NoteFile()
{
	pugi::xml_parse_result result = xml_doc.load_file(filePath.c_str());

}

std::vector<std::shared_ptr<NF::StickyNote>> NF::NoteFile::getStickyNote()
{
	std::vector<std::shared_ptr<StickyNote>> notes;
	PropertyString stickyNoteType("stickyNoteType", "");
	for (pugi::xml_node node : xml_doc)
	{
		stickyNoteType.loadValue(node);
		std::shared_ptr<StickyNote> note = creatStickyNote(stickyNoteType.getValue());
		note->loadNode(node);
		notes.push_back(note);
	}
	return notes;
}

pugi::xml_node NF::NoteFile::creatNode()
{
 	return xml_doc.append_child("StickyNote");
}

bool NF::NoteFile::save()
{
	return xml_doc.save_file(filePath.c_str());
}

std::shared_ptr<NF::StickyNote> NF::NoteFile::creatStickyNote(const std::string& stickyNoteType)
{
	if (stickyNoteType == "contentText")
	{
		return std::shared_ptr<StickyNote>(new StickyNoteText());
	}else if (stickyNoteType == "contentToDoList")
	{
		return std::shared_ptr<StickyNote>(new StickyNoteToDoList());
	}
	else {
		return std::shared_ptr<StickyNote>(new StickyNoteText());
	}
}

void NF::NoteFile::setFilePath(const std::string& path)
{
	this->filePath = path;
	xml_doc.load_file(filePath.c_str());
}

std::string NF::NoteFile::getFilePath()
{
	return filePath;
}

std::shared_ptr<NF::NoteFile> NF::NoteFile::instance;

NF::NoteFile::~NoteFile()
{
	
}

NF::StickyNote::StickyNote()
	:posX(0),posY(0),
	times(""),width(320),height(320),
	visible(true), backgroundColorIndex(0),state(NORMAL),
	contentIsNull(true),backgroundColor("#FFF7D1"),version(VERSION),stickyNoteType("stickyNoteType", "contentText")
{
	guid = creatGUID();

	times = getTimes();
}



bool NF::StickyNote::operator==(const StickyNote& note)
{
	//判等只判断GUID是否相等
	if (this->guid == note.guid)
		return true;
	return false;
}

void NF::StickyNote::loadNode(pugi::xml_node& node)
{
	stickyNoteType.loadValue(node);
	posX = node.attribute("posX").as_int();
	posY = node.attribute("posY").as_int();
	width = node.attribute("width").as_int();
	height = node.attribute("height").as_int();
	state =(State)node.attribute("state").as_int(0);
	backgroundColorIndex = node.attribute("backgroundColorIndex").as_int(0);

	backgroundColor = node.attribute("backgroundColor").as_string();
	times = node.attribute("times").as_string();
	customColor = node.attribute("customColor").as_string();
	visible = node.attribute("visible").as_bool();
	version = node.attribute("version").as_double(VERSION);

	//兼容之前没有GUID的文件
	std::string id = node.attribute("GUID").as_string();
	if (!id.empty())
		guid = id;
	
	this->node = node;
}

void NF::StickyNote::toNode(pugi::xml_node& node)
{
	//如果内容为空，则视为删除
	if (contentIsNull)
	{
		auto pnode = node.parent();
		if (!pnode)
			return;
		pnode.remove_child(node);
		node = xml_node();
		return;
	}

	node.remove_attributes();
	node.append_attribute("posX")= posX;
	node.append_attribute("posY") = posY;
	node.append_attribute("width") = width;
	node.append_attribute("height") = height;
	node.append_attribute("backgroundColorIndex") = backgroundColorIndex;
	node.append_attribute("times") = times.c_str();
	node.append_attribute("visible") = visible;
	node.append_attribute("state") = state;
	node.append_attribute("GUID") = guid.c_str();

	node.append_attribute("customColor") = customColor.c_str();
	node.append_attribute("backgroundColor") = backgroundColor.c_str();
	node.append_attribute("version") = version;
	stickyNoteType.saveValue(node);
}

void NF::StickyNote::save()
{
	toNode(node);
}

NF::StickyNote NF::StickyNote::copy()
{
	StickyNote nt(*this);
	nt.node = xml_node();
	nt.guid = creatGUID();
	nt.times = getTimes();
	return nt;
}


std::string NF::StickyNote::creatGUID()
{

	static char buf[64] = { 0 };
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		_snprintf(buf, sizeof(buf)
				, "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"
				, guid.Data1
				, guid.Data2
				, guid.Data3
				, guid.Data4[0], guid.Data4[1]
				, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
				, guid.Data4[6], guid.Data4[7]
		);
	}
	return std::string(buf);
}

void NF::StickyNote::deleteFile()
{
	if (node.empty())
		return;
	auto pnode = node.parent();
	if (!pnode)
		return;
	pnode.remove_child(node);
	auto file = NoteFile::GetInstance();
	file->save();
	node = xml_node();
}

std::string NF::StickyNote::getTimes()
{
		time_t tt;
		struct tm* t;
		tt = time(0);
		t = localtime(&tt);
		char char_time[50] = {};
		sprintf(char_time, " %04d-%02d-%02d %02d:%02d:%02d", t->tm_year + 1900,
			t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		std::string str_system_time = static_cast<std::string>(char_time);
		return str_system_time;
}

bool NF::StickyNote::isSaved()
{
	if (node.empty())
		return false;
	return true;
}

NF::StickyNoteText::StickyNoteText()
	:contentText("contentText","")
{
	stickyNoteType.setValue(getStickyNoteType());
}


void NF::StickyNoteText::loadNode(xml_node& node)
{
	StickyNote::loadNode(node);
	contentText.loadValue(node);
	//兼容之前版本的数据
	if (contentText.getValue().empty())
	{
		contentText = node.attribute("content").as_string();
	}
}

void NF::StickyNoteText::toNode(xml_node& node)
{
	StickyNote::toNode(node);
	contentText.saveValue(node);
}

std::string NF::StickyNoteText::getStickyNoteType()
{
	static const std::string  type("contentText");
	return type;
}

NF::StickyNoteToDoListItem::StickyNoteToDoListItem()
	:contentText("contentText",""),isFilished("isFilished",false)
	,createDate("createDate",""),finishedDate("finishedDate","")
{
	createDate = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString();
}

bool NF::StickyNoteToDoListItem::load(xml_node& node)
{
	if (std::string(node.name()) != "StickyNoteToDoListItem")
		return false;
	contentText.loadValue(node);
	isFilished.loadValue(node);
	createDate.loadValue(node);
	finishedDate.loadValue(node);
	return true;
}

bool NF::StickyNoteToDoListItem::save(xml_node& node)
{
	auto stickyNoteToDoListItem = node.append_child("StickyNoteToDoListItem");
	contentText.saveValue(stickyNoteToDoListItem);
	isFilished.saveValue(stickyNoteToDoListItem);
	createDate.saveValue(stickyNoteToDoListItem);
	finishedDate.saveValue(stickyNoteToDoListItem);
	return true;
}

std::list<NF::StickyNoteToDoListItem> NF::StickyNoteToDoListItem::loadAll(xml_node& node)
{
	std::list<StickyNoteToDoListItem> listItem;
	for (auto child : node)
	{
		StickyNoteToDoListItem item;
		if (item.load(child))
			listItem.push_back(item);
	}

	return listItem;
}

NF::StickyNoteToDoList::StickyNoteToDoList()
	:isSplit("isSplit",false)
{
	stickyNoteType.setValue(getStickyNoteType());
}

void NF::StickyNoteToDoList::loadNode(xml_node& node)
{
	StickyNote::loadNode(node);
	isSplit.loadValue(node);
	toDoListItems = StickyNoteToDoListItem::loadAll(node);
}

void NF::StickyNoteToDoList::toNode(xml_node& node)
{
	StickyNote::toNode(node);
	isSplit.saveValue(node);
	while (node.remove_child("StickyNoteToDoListItem"));
	for (auto item : toDoListItems)
	{
		item.save(node);
	}
}

std::string NF::StickyNoteToDoList::getStickyNoteType()
{
	static const std::string  type("contentToDoList");
	return type;
}
