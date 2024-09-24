#pragma once
#include "ExportConfig.hpp"
#include "xml/pugixml.hpp"
#include "Property.h"
#include <memory>
#include <mutex>
#include <vector>
#include <list>
namespace NF {
	using namespace pugi;
	class NOTE_FILE_EXPORT StickyNote
	{
	public:
		enum State {
			NORMAL = 0, //正常窗口
			PIN_ICON_LAYER, //固定到图标层
			PIN_ALWAYS_TOP //固定到屏幕
		};
	public:
		StickyNote();
		~StickyNote() = default;
		bool operator==(const StickyNote& note);

		//从xml中载入数据
		virtual void loadNode(xml_node& node);
		//存储数据到xml
		virtual void toNode(xml_node& node);
		//存储数据到xml，必须保证xml_node是一个有效的
		void save();
		//复制数据，不复制node
		StickyNote copy();
		//生成guid
		static std::string creatGUID();
		//删除文件
		void deleteFile();
		//获取当前系统事件
		std::string getTimes();
		//是否保存
		bool isSaved();

	public:
		int posX,posY; //窗口位置
		int width, height;//窗口大小
		std::string times; //创建时间
		xml_node node;	//文件节点
		bool visible;  //是否可用
		State state;	//窗口状态
		int backgroundColorIndex; //背景色索引
		std::string backgroundColor; //背景颜色
		std::string customColor;	//自定义颜色值
		bool contentIsNull;	//内容是否为空
		std::string guid; //便笺标识符
		double version;//版本号
		PropertyString stickyNoteType;
	};

	class NOTE_FILE_EXPORT StickyNoteText :public StickyNote {
	public:
		StickyNoteText();
		//从xml中载入数据
		virtual void loadNode(xml_node& node) override;
		//存储数据到xml
		virtual void toNode(xml_node& node) override;
		//返回类型
		static std::string getStickyNoteType();
	public:
		PropertyString contentText;
	};

	struct NOTE_FILE_EXPORT StickyNoteToDoListItem{
	public:
		StickyNoteToDoListItem();
		~StickyNoteToDoListItem() = default;
		bool load(xml_node& node);
		bool save(xml_node& node);
		static std::list<StickyNoteToDoListItem> loadAll(xml_node& node);
		PropertyString contentText,createDate,finishedDate;
		PropertyBool isFilished;
	};
	class NOTE_FILE_EXPORT StickyNoteToDoList :public StickyNote {
	public:
		StickyNoteToDoList();
		//从xml中载入数据
		virtual void loadNode(xml_node& node) override;
		//存储数据到xml
		virtual void toNode(xml_node& node) override;
		//返回类型
		static std::string getStickyNoteType();
	public:
		std::list<StickyNoteToDoListItem> toDoListItems;
		//显示时是否应用分类 -> 暂时只有按照是否完成分类
		PropertyBool isSplit;

	};

	class NOTE_FILE_EXPORT NoteFile {
	public:
		~NoteFile();
		NoteFile(const NoteFile& file) = delete;
		NoteFile& operator=(const NoteFile&) = delete;
		static std::shared_ptr<NoteFile> GetInstance() {
			static std::once_flag flag;

			std::call_once(flag, [&]() {
				instance.reset(new NoteFile);
				});
			return instance;
		}
	private:
		NoteFile();

	public:
		std::vector<std::shared_ptr<StickyNote>> getStickyNote();
		xml_node creatNode();
		bool save();
		static std::shared_ptr<StickyNote> creatStickyNote(const std::string& stickyNoteType);
	public:
		//get set
		void setFilePath(const std::string& path);
		std::string getFilePath();
	private:
		static std::shared_ptr<NoteFile> instance;

		pugi::xml_document xml_doc;

		std::string filePath = "test.xml";
	};

}