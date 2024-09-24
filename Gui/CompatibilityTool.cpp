#include "CompatibilityTool.h"
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <iostream>
void CompatibilityTool::moveConfigFile()
{
	QDir dir;
	auto homePath = dir.homePath();
	homePath += "/.StickyNote";
	QString oldNoteFile(homePath + "/note.xml");
	QString oldConfigFile(homePath + "/config.cfg");
	
	auto newPath = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath() +"/../data";
	auto stdnew = newPath.toStdString();
 	QString newNoteFile(newPath +"/note.xml");
	QString newConfigFile(newPath + "/config.cfg");

	//如果文件已存在则不需要迁移
	if (QFile(newNoteFile).exists() && QFile(newConfigFile).exists())
		return;
	//如果旧文件不存在，也不需要迁移
	if (!QFile(oldNoteFile).exists() || !QFile(oldConfigFile).exists())
		return;
	
	{
		QDir destinationDir(newPath);
		if (!destinationDir.exists()) {
			if (!destinationDir.mkpath(destinationDir.absolutePath())) {
				std::cerr << "Failed to create destination directory! path: "
					<< destinationDir.absolutePath().toStdString()
					<< std::endl;
			}
		}
		QFile file(oldNoteFile);
		if (file.copy(newNoteFile)) {
			std::cerr << "File copied successfully!" << std::endl;
		}
		else {
			std::cerr << "Failed to copy file!"
				<< file.errorString().toStdString() << std::endl;
		}
	
	}

	{
		QDir destinationDir(newPath);
		if (!destinationDir.exists()) {
			if (!destinationDir.mkpath(destinationDir.absolutePath())) {
				std::cerr << "Failed to create destination directory! path: "
					<< destinationDir.absolutePath().toStdString()
					<< std::endl;
			}
		}
		QFile file(oldConfigFile);
		if (file.copy(newConfigFile)) {
			std::cerr << "File copied successfully!  " 
				<< std::endl;
		}
		else {
			std::cerr << "Failed to copy file!" 
				<< file.errorString().toStdString() << std::endl;
		}

	}


}

