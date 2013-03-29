#include "JConfig.h"

namespace autosquid
{
	void WriteInputConfiguration( InputConfiguration& _conf, QString f_s )
	{
		QFile file (f_s);

		if (!file.open(QIODevice::WriteOnly))
		{
			qDebug()<<"error Open file"<<f_s;
			return;
		}

		QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
		xmlWriter->setDevice(&file);
		xmlWriter->setAutoFormatting(true);

		xmlWriter->writeStartDocument();
		xmlWriter->writeStartElement("config");

		const QMetaObject* metaObj = _conf.metaObject();
		int nField = metaObj->propertyCount();

		for (int i=0; i<nField; ++i)
		{
			QMetaProperty metaProp = metaObj->property(i);
			xmlWriter->writeTextElement(metaProp.name(),metaProp.read(&_conf).toString());
		}

		xmlWriter->writeEndElement();
		xmlWriter->writeEndDocument();
	}

	bool ReadInputConfiguration(InputConfiguration & _conf, QString f_s)
	{
		QFile file (f_s);

		if (!file.open(QIODevice::ReadOnly))
		{
			qDebug()<<"error Open file"<<f_s;
			return false;
		}

		QXmlStreamReader xml(&file);
		const QMetaObject* metaObj = _conf.metaObject();
		int nField = metaObj->propertyCount();

		//! in this way, the xml file can be changed by hand and fields of InputConfiguration are filled
		//! automatically without 
		QString prop_name;
		QString prop_value;
		while (!xml.atEnd() && !xml.hasError())
		{
			QXmlStreamReader::TokenType token = xml.readNext();		

			if (token == QXmlStreamReader::StartDocument)
				continue;

			if (token == QXmlStreamReader::StartElement)
			{
				prop_name = xml.name().toString().trimmed();
				continue;
			}
			if(token == QXmlStreamReader::Characters)
			{
				prop_value = xml.text().toString().trimmed();
				if (prop_value.isEmpty()) continue;
				int i;
				for (i=0; i<nField; ++i)
				{
					QMetaProperty metaProp = metaObj->property(i);
					if (metaProp.name() == prop_name)
					{
						metaProp.write(&_conf,prop_value);
						break;
					}
				}
				if (i== nField){
					qDebug()<<"Property "<<prop_name<<" in xml has no correspondence in InputConfiguration class";
				}
			}
		}

		return true;
	}

	std::ostream& operator<<( std::ostream& out, const InputConfiguration& _conf )
	{
		const QMetaObject* metaObj = _conf.metaObject();
		int nField = metaObj->propertyCount();

		for (int i=0; i<nField; ++i)
		{
			QMetaProperty metaProp = metaObj->property(i);
			out<<metaProp.name()<<":"<<metaProp.read(&_conf).toString().toStdString()<<std::endl;
		}
		return out;
	}
}


