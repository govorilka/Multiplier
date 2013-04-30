#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

#include <QtGui/QApplication>
#include <QtGui/QImage>

bool convertToPremultiplied(const QString& filename)
{
	QFileInfo fInfo(filename);
	if (!fInfo.exists())
	{
		qDebug() << "File not found";
		return false;
	}

	QImage image;
	if (!image.load(fInfo.absoluteFilePath()))
	{
		qDebug() << "Cannot load image file";
		return false;
	}

	image = image.convertToFormat(QImage::Format_ARGB32);

	uchar* data = image.bits();

	int byte_count = image.byteCount();
	for (int i = 0; i < byte_count; i += 4)
	{
		uchar& blue = *data++;
		uchar& green = *data++;
		uchar& red = *data++;

		qreal transparency = static_cast<qreal>(*data++) / 255.0;

		red = qRound(transparency * static_cast<qreal>(red));
		green = qRound(transparency * static_cast<qreal>(green));
		blue = qRound(transparency * static_cast<qreal>(blue));
	}

	if (!image.save(QDir::toNativeSeparators(fInfo.absolutePath() + "/pa_" + fInfo.fileName())))
	{
		qDebug() << "Cannot save file";
		return false;
	}

	return true;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	if (a.arguments().size() < 2)
	{
		qDebug() << "Filename is not specified";
		return -1;
	}

	if (!convertToPremultiplied(a.arguments().at(1)))
	{
		return -1;
	}

	return 0;
}
