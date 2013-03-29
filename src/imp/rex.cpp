/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 9 of the cookbook:  
   Computer Vision Programming using the OpenCV Library. 
   by Robert Laganiere, Packt Publishing, 2011.

   This program is free software; permission is hereby granted to use, copy, modify, 
   and distribute this source code, or portions thereof, for any purpose, without fee, 
   subject to the restriction that the copyright notice may not be removed 
   or altered from any source or altered source distribution. 
   The software is released on an as-is basis and without any warranties of any kind. 
   In particular, the software is not guaranteed to be fault-tolerant or free from failure. 
   The author disclaims all warranties with regard to this software, any use, 
   and any consequent failure, is purely the responsibility of the user.
 
   Copyright (C) 2010-2011 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/


#include <iostream>
#include <iomanip>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>


#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileInfo>

#include "config/config.h"
#include "CameraCalibrator.h"

#include "ConfigurationManager/JConfig.h"

int rex_main(int argc, char * argv[])
{
	cv::namedWindow("Image");
	cv::Mat image;
	std::vector<std::string> filelist;

	autosquid::InputConfiguration conf;

	QString conf_f;

#ifdef QUICK_DEBUG 
	conf_f = QString::fromAscii(CONFIG_FILE);
#else
	conf_f = QFileDialog::getOpenFileName(0,"Open Configuration File","",("xml (*.xml)"));
#endif

	if (conf_f.isEmpty() || QFileInfo(conf_f).exists()==false) 
	{
		std::cerr<<"error loading configuration file: check if "<<conf_f.toStdString()<<" exists"<<std::endl;
		return -1;
	}

	autosquid::ReadInputConfiguration(conf,conf_f);


	QString intrinsic_dir = conf.getIntrinsicChessBoard();
	QDir dir(intrinsic_dir);

	 QStringList img_filters;
     img_filters << "*.bmp" << "*.jpg" << "*.png";

	QStringList intrinsic_fs = dir.entryList(img_filters,QDir::Files,QDir::SortFlag::Time);

	QString f;
	foreach(f,intrinsic_fs)
		std::cout<<f.toStdString()<<std::endl;
	
	return 0;
	// generate list of chessboard image filename
	for (int i=1; i<=0; i++) {

		std::stringstream str;
		str << "../chessboards/chessboard" << std::setw(2) << std::setfill('0') << i << ".jpg";
		std::cout << str.str() << std::endl;

		filelist.push_back(str.str());
		image= cv::imread(str.str(),0);
		cv::imshow("Image",image);
	
		 cv::waitKey(100);
	}

	// Create calibrator object
    CameraCalibrator cameraCalibrator;
	// add the corners from the chessboard
	cv::Size boardSize(6,4);
	cameraCalibrator.addChessboardPoints(
		filelist,	// filenames of chessboard image
		boardSize);	// size of chessboard
		// calibrate the camera
    //	cameraCalibrator.setCalibrationFlag(true,true);
	cameraCalibrator.calibrate(image.size());

    // Image Undistortion
    image = cv::imread(filelist[6]);
	cv::Mat uImage= cameraCalibrator.remap(image);

	// display camera matrix
	cv::Mat cameraMatrix= cameraCalibrator.getCameraMatrix();
	std::cout << " Camera intrinsic: " << cameraMatrix.rows << "x" << cameraMatrix.cols << std::endl;
	std::cout << cameraMatrix.at<double>(0,0) << " " << cameraMatrix.at<double>(0,1) << " " << cameraMatrix.at<double>(0,2) << std::endl;
	std::cout << cameraMatrix.at<double>(1,0) << " " << cameraMatrix.at<double>(1,1) << " " << cameraMatrix.at<double>(1,2) << std::endl;
	std::cout << cameraMatrix.at<double>(2,0) << " " << cameraMatrix.at<double>(2,1) << " " << cameraMatrix.at<double>(2,2) << std::endl;

    imshow("Original Image", image);
    imshow("Undistorted Image", uImage);

	cv::waitKey();
	return 0;
}