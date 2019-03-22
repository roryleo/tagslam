/* -*-c++-*--------------------------------------------------------------------
 * 2019 Bernd Pfrommer bernd.pfrommer@gmail.com
 */

#pragma once
#include "tagslam/graph.h"
#include "tagslam/value_key.h"
#include "tagslam/factor_key.h"
#include "tagslam/geometry.h"

namespace tagslam {
  class Optimizer {
  public:
    Optimizer() {}
    virtual ~Optimizer() {};

    virtual void optimize() = 0;
    virtual void optimizeFullGraph(bool force = false) = 0;
    virtual void setErrorThreshold(double th) = 0;


    // retrieves the optimized pose for a given key
    virtual Transform getPose(ValueKey key) = 0;
    // adds the starting guess for a new value (e.g. camera pose)
    virtual ValueKey addPose(const Transform &pose) = 0;
    // relative pose prior, i.e. err = ||Pose(key1) - deltaPose * Pose(key2)||
    virtual FactorKey addRelativePosePrior(ValueKey key1, ValueKey key2,
                                           const PoseWithNoise &deltaPose) = 0;
    // absolute pose prior, i.e. err = ||Pose(key) - pose||
    virtual FactorKey addAbsolutePosePrior(ValueKey key,
                                           const PoseWithNoise &pose) = 0;
    // tag projection factor:
    //    T_c_o   = T_c_r * T_r_w * T_w_b * T_b_o;
    //    u_proj_i = K * rad_dist(T_c_o * X_i)   where i = 1..4 (corners)
    //    err = sum_i  ||u_proj_i - u||
    virtual FactorKey addTagProjectionFactor(
      // u = image points  (2d)
      const Eigen::Matrix<double, 4, 2> &u,
      // X = object points (3d, but in plane with z = 0)
      const Eigen::Matrix<double, 4, 3> &X,
      const std::string &cameraName,
      const CameraIntrinsics2 &ci,
      double pixelNoise,
      ValueKey T_c_r, ValueKey T_r_w, ValueKey T_w_b, ValueKey T_b_o) = 0;
  private:
  };
}
