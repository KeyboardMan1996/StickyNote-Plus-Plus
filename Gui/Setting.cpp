#include "Setting.h"
#include "Property.h"
#include "version.hpp"
#include "NoteFile.h"
#include "ActionCommand.h"
#include <QFile>


std::string NGui::Setting::defaultPath("config.xml");
std::shared_ptr< NGui::Setting> NGui::Setting::_instance;

NGui::Setting::Setting()
{
	initProperty();
	loadProperty();
}

NGui::Setting::~Setting()
{

}

void NGui::Setting::loadProperty()
{
	NF::Setting::loadProperty(defaultPath);
}

void NGui::Setting::saveProperty()
{
	NF::Setting::saveProperty(defaultPath);
}

void NGui::Setting::setDefaultConfigPath(const std::string& path)
{
	defaultPath = path;
}

std::string NGui::Setting::getDefaultConfigPath()
{
	return defaultPath;
}

bool NGui::Setting::defaultPathExist()
{
	QFile file(QString::fromStdString(defaultPath));
	return file.exists();
}

std::shared_ptr<NGui::Setting> NGui::Setting::GetInstance()
{
	static std::once_flag flag;
	std::call_once(flag, [&] (){
		_instance.reset(new Setting());
	});
	return _instance;
}

void NGui::Setting::initProperty()
{
	stickyNoteToolbarAutoPop = new NF::PropertyBool("stickyNoteToolbarAutoPop",true);
	addProperty(stickyNoteToolbarAutoPop);
	stickyNoteToolMod = new NF::PropertyBool("stickyNoteToolMod", true);
	addProperty(stickyNoteToolMod);
	historyWidgetShortcut = new NF::PropertyString("historyWidgetShortcut", "Ctrl+Q");
	addProperty(historyWidgetShortcut);
	ignoreVersion = new NF::PropertyDouble("ignoreVersion", VERSION);
	addProperty(ignoreVersion);
	autoUpdate = new NF::PropertyBool("autoUpdate", true);
	addProperty(autoUpdate);
	userID = new NF::PropertyString("userID", NF::StickyNote::creatGUID());
	addProperty(userID);
	autoMagnetic = new NF::PropertyBool("autoMagnetic", true);
	addProperty(autoMagnetic);
	magneticThreshold = new NF::PropertyInt("magneticThreshold", 40);
	addProperty(magneticThreshold);
	magneticDistance = new NF::PropertyInt("magneticDistance", 5);
	addProperty(magneticDistance);
	stickyNoteFont = new NF::PropertyString("stickyNoteFont", "Microsoft YaHei,13,-1,5,50,0,0,0,0,0");
	addProperty(stickyNoteFont);
	imageSavePath = new NF::PropertyString("imageSavePath","../image");
	addProperty(imageSavePath);
	transparentWidget = new NF::PropertyBool("transparentWidget", true);
	addProperty(transparentWidget);
	pinIconLayer = new NF::PropertyBool("pinIconLayer", true);
	addProperty(pinIconLayer);
	stickyNoteHeight = new NF::PropertyInt("stickyNoteHeight", 400);
	stickyNoteWidth = new NF::PropertyInt("stickyNoteWidth", 380);
	addProperty(stickyNoteWidth);
	addProperty(stickyNoteHeight);
	visitiUrl = new NF::PropertyBool("visitiUrl", true);
	addProperty(visitiUrl);

	std::vector<std::string> cmds = ActionCmdFactory::GetInstance()->getAllCmdName();
	for (std::string cmdName : cmds)
	{
		//在这里设置一些命令的初始状态
		if(cmdName == "cmd_close")
			continue;
		auto cmdPr = new NF::PropertyBool(cmdName, true);

		if (cmdName == "cmd_text_to_left")
			cmdPr->setValue(false);
		else if (cmdName == "cmd_text_to_right")
			cmdPr->setValue(false);
		else if (cmdName == "cmd_font_size_up")
			cmdPr->setValue(false);
		else if (cmdName == "cmd_font_size_lower")
			cmdPr->setValue(false);
		else if (cmdName == "cmd_font_italic")
			cmdPr->setValue(false);
		else if (cmdName == "cmd_font_under_line")
			cmdPr->setValue(false);
		else if (cmdName == "cmd_font_color")
			cmdPr->setValue(false);

		cmdState[cmdName] = cmdPr;
		addProperty(cmdPr);
	}
}

