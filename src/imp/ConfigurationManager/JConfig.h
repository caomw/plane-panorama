#pragma  once

#include <QObject>
#include <QMetaObject>
#include <QString>
#include <QMetaProperty>
#include <QDebug>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include <QFile>

#include <ostream>


namespace autosquid{

class  InputConfiguration : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString intrinsic_chessboard_dir READ getIntrinsicChessBoard WRITE setIntrinsicChessBoard);
	Q_PROPERTY(QString image_dir READ getImageDir WRITE setImageDir);
	Q_PROPERTY(QString model_path READ getModelPath WRITE setModelPath);
	Q_PROPERTY(QString neighbor_cache READ getNeighborCache WRITE setNeighborCache);
	Q_PROPERTY(QString vis_path READ getVisPath WRITE setVisPath);
	Q_PROPERTY(QString mask_dir READ getMaskDir WRITE setMaskDir);
	Q_PROPERTY(QString target_cam_file_path READ getTargetCamFilePath WRITE setTargetCamFilePath);
	Q_PROPERTY(QString vis_texture_dir READ getVisTextureDir WRITE setVisTextureDir);
	Q_PROPERTY(QString out_dir READ getOutDir WRITE setOutDir);
	Q_PROPERTY(QString temp_dir READ getTempDir WRITE setTempDir);
	Q_PROPERTY(QString vis_on_target_cams READ getVisOnTargetCams WRITE setVisOnTargetCams);

public:
	QString getIntrinsicChessBoard(){	return intrinsic_chessboard_dir; }
	void setIntrinsicChessBoard(const QString& _intrinsic_chessboard_dir){	intrinsic_chessboard_dir = _intrinsic_chessboard_dir;	};
	
	QString getNeighborCache(){	return neighbor_cache; }
	void setNeighborCache(const QString& _neighbor_cache){	neighbor_cache = _neighbor_cache;	};

	QString getImageDir(){return image_dir;}
	void setImageDir(const QString& _img_dir){image_dir = _img_dir;}

	QString getModelPath(){ return model_path;}
	void setModelPath(const QString& _modelPath){model_path = _modelPath;}

	QString  getVisPath(){ return vis_path;}
	void setVisPath(const QString &_vis_path){vis_path = _vis_path;}

	QString getMaskDir(){return mask_dir;}
	void setMaskDir(const QString& _mask_dir){mask_dir = _mask_dir;}

	QString getTargetCamFilePath(){return target_cam_file_path;}
	void setTargetCamFilePath(const QString& _target_cam_file_path){target_cam_file_path = _target_cam_file_path;}

	QString getVisTextureDir(){return vis_texture_dir;}
	void setVisTextureDir(const QString& _vis_texture_dir){vis_texture_dir = _vis_texture_dir;}

	QString getOutDir(){return out_dir;}
	void setOutDir(const QString& _out_dir){ out_dir = _out_dir;}

	
	QString getTempDir(){ return temp_dir;}
	void setTempDir(const QString& _temp_dir){temp_dir = _temp_dir;}

	QString getVisOnTargetCams(){ return vis_on_target_cams;}
	void setVisOnTargetCams(const QString& _vis_on_target_cams){vis_on_target_cams = _vis_on_target_cams;}


	QString intrinsic_chessboard_dir;
	QString image_dir;
	QString model_path;
	QString neighbor_cache;
	QString vis_path ;
	QString mask_dir ;
	QString target_cam_file_path ;
	QString out_dir;
	QString temp_dir;
	QString vis_texture_dir;

	QString vis_on_target_cams;
};

bool ReadInputConfiguration(InputConfiguration& _conf, QString f_s);

void WriteInputConfiguration(InputConfiguration& _conf, QString f_s);

std::ostream& operator<<(std::ostream& out, const InputConfiguration& _conf);
}

