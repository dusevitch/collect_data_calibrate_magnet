#ifndef POLARISTRANSFORMMATRIX_H
#define POLARISTRANSFORMMATRIX_H

#pragma once
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Core>
struct polarisTransformMatrix{
    Eigen::Matrix3d rot_mat = Eigen::MatrixXd::Identity(3,3);
    Eigen::Matrix3d inv_rot_mat = Eigen::MatrixXd::Identity(3,3);
    Eigen::Vector3d pos = Eigen::Vector3d(-1000.0,-1000.0,-1000.0);
    Eigen::Matrix4d trans_mat = Eigen::MatrixXd::Identity(4,4);
    Eigen::Matrix4d inv_trans_mat = Eigen::MatrixXd::Identity(4,4);
};

//struct to hold position and rotation values
struct pos_rot{
    double deg = 0;
    Eigen::Vector3d pos = Eigen::Vector3d(-1000.0,-1000.0,-1000.0);
    Eigen::Matrix3d rot = Eigen::MatrixXd::Identity(3,3);
};

#endif // POLARISTRANSFORMMATRIX_H
