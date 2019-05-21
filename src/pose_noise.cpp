/* -*-c++-*--------------------------------------------------------------------
 * 2018 Bernd Pfrommer bernd.pfrommer@gmail.com
 */

#include "tagslam/pose_noise.h"
#include <Eigen/Cholesky>

#include <iostream>

namespace tagslam {
  // static function
  PoseNoise PoseNoise::make(const Point3d &a, const Point3d &p) {
    Matrix6d m = Matrix6d::Zero();
    m.diagonal() << a(0),a(1),a(2),p(0),p(1),p(2);
    return (PoseNoise(m, true));
  }

  // static function
  PoseNoise PoseNoise::make(double a, double p) {
    return (make(Point3d(a, a, a), Point3d(p, p, p)));
  }
 
  static const Matrix6d sqrt_info_to_sigma(const Matrix6d &R) {
    // TODO: test this! Is it working at all???
    // Could be just as well R * R.transpose()
    const Matrix6d rsqi = (R.transpose() * R).inverse();
    return (rsqi);
  }

  // static method
  PoseNoise PoseNoise::makeFromR(const Matrix6d &R) {
    const Matrix6d sigma = sqrt_info_to_sigma(R);
    return (PoseNoise(sigma, false /*isdiag*/));
  }

  Eigen::Matrix<double, 6, 1> PoseNoise::getDiagonal() const {
    return (noise.diagonal());
  }
  
  Matrix6d PoseNoise::convertToR() const {
    //Sigma = R * R^T
    // Cholesky decomposition: sigma = L * L.transpose();
    const Matrix6d ni = noise.inverse();
    Eigen::LLT<Matrix6d> llt(ni);
    const Matrix6d U = llt.matrixU();
    return (U);
  }

  std::ostream &operator<<(std::ostream &os, const PoseNoise &pn) {
    os << "is_diagonal: " << pn.isDiagonal << std::endl << pn.noise;
    return (os);
  }
    
}  // namespace
