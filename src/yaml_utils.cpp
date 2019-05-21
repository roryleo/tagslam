/* -*-c++-*--------------------------------------------------------------------
 * 2018 Bernd Pfrommer bernd.pfrommer@gmail.com
 */

#include "tagslam/yaml_utils.h"
#include <boost/range/irange.hpp>
#include <XmlRpcException.h>
#include <Eigen/Geometry>

namespace tagslam {
  using boost::irange;

  namespace yaml_utils {
    static void write_vec(std::ostream &of,
                          const std::string &prefix,
                          double x, double y, double z) {
      const int p(8);
      of.precision(p);
      of << prefix << "x: " << std::fixed << x << std::endl;
      of << prefix << "y: " << std::fixed << y << std::endl;
      of << prefix << "z: " << std::fixed << z << std::endl;
    }


    void write_pose(std::ostream &of, const std::string &prefix,
                    const Transform &pose,
                    const PoseNoise &n, bool writeNoise) {
      Eigen::AngleAxisd aa(pose.linear());
      Point3d r = aa.angle() * aa.axis();
      Point3d t(pose.translation());
      const std::string pps = prefix + "  ";
      of << prefix << "position:" << std::endl;
      write_vec(of, pps, t(0), t(1), t(2));
      of << prefix << "rotation:" << std::endl;
      write_vec(of, pps, r(0), r(1), r(2));
      if (writeNoise) {
        Eigen::Matrix<double, 6, 1> diag = n.getDiagonal();
        of << prefix << "position_noise:" << std::endl;
        write_vec(of, pps, diag(3),diag(4),diag(5));
        of << prefix << "rotation_noise:" << std::endl;
        write_vec(of, pps, diag(0),diag(1),diag(2));
      }
    }

    void write_pose_with_covariance(std::ostream &of,
                                    const std::string &prefix,
                                    const Transform &pose,
                                    const PoseNoise &n) {
      Eigen::AngleAxisd aa;
      aa.fromRotationMatrix(pose.rotation());
      Eigen::Vector3d r = aa.angle() * aa.axis();
      Eigen::Vector3d t = pose.translation();
      const std::string pps = prefix + "  ";
      of << prefix << "position:" << std::endl;
      write_vec(of, pps, t(0), t(1), t(2));
      of << prefix << "rotation:" << std::endl;
      write_vec(of, pps, r(0), r(1), r(2));
      of << prefix << "R:" << std::endl;
      const auto R = n.convertToR();
      of << prefix << "  [ ";
      for (const auto i: irange(0l, R.rows())) {
        for (const auto j: irange(0l, R.cols())) {
          of << R(i, j);
          if (i != R.rows() - 1 || j != R.cols() - 1) {
            of << ", ";
          }
        }
        if (i != R.rows() - 1) {
          of << std::endl << prefix << "    ";
        }
      }
      of << " ]" << std::endl;
    }
  }
}  // namespace
