#include "ImageSave.h"
#include <QDate>
#include <iostream>
#include <QDir>
#include <QFile>

QString NF::ImageSave::defaultFolder = "./image";
const QString baseFileName = "sticky_note_image_";

NF::ImageSave::ImageSave()
	:fileName("")
{
	date = QDate::currentDate().toString(Qt::ISODate);
	disposFolderExist();
}

NF::ImageSave::~ImageSave()
{

}

bool NF::ImageSave::save(const QImage& image, QString endName /*= ".jpg"*/)
{
	int fileCount = getFileCount();
	fileName = baseFileName + QString::number(fileCount) + endName;
	QString filePath = path + "/" + fileName;

	const int maxSaveCount = 100;	//如果最大检查次数，以免陷入死循环
	{
		int  i = 0;
		while (i < maxSaveCount && disposFileExist(filePath))
		{
			fileCount++;
			fileName = baseFileName + QString::number(fileCount) + endName;
			filePath = path + "/" + fileName;
			i++;
		}
	}
	//如果文件已存在 保存失败
	if (disposFileExist(filePath))
	{
		std::cerr << "NF::ImageSave::save save count exceeding maximum limit,save filed!" << std::endl;
		return false;
	}

	if(image.save(filePath))
		return true;
	std::cerr << "NF::ImageSave::save filed! path: " << filePath.toStdString()<< std::endl;
	return false;
}

QString NF::ImageSave::getPath()
{
	return path + "/" + fileName;
}

void NF::ImageSave::setDefaultFolder(const QString& folder)
{
	defaultFolder = folder;
}

void NF::ImageSave::disposFolderExist()
{
	path = defaultFolder + "/date-" +date + "/";
	QDir dir(path);
	path = dir.absolutePath();
	if (dir.exists())
		return;
	if (!dir.mkpath(path))
		std::cerr << "NF::ImageSave::disposFolderExist() make dir failed! path: " << path.toStdString() << std::endl;
}

int NF::ImageSave::getFileCount()
{
	QDir dir(path);
	return dir.count();
}

bool NF::ImageSave::disposFileExist(const QString& fileP)
{
	QFile file(fileP);
	return file.exists();
}
