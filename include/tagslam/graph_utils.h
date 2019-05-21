/* -*-c++-*--------------------------------------------------------------------
 * 2019 Bernd Pfrommer bernd.pfrommer@gmail.com
 */

#include "tagslam/graph.h"
#include "tagslam/body.h"
#include "tagslam/tag2.h"
#include "tagslam/pose_with_noise.h"
#include "tagslam/camera2.h"

#include <ros/ros.h>

#include <string>

#pragma once

namespace tagslam {
  namespace graph_utils {
    using std::string;
    // convenience functions for adding to graph
    void add_pose_maybe_with_prior(Graph *g, const ros::Time &t,
                                   const string &name,
                                   const PoseWithNoise &pwn, bool isCamPose);
    void add_tag(Graph *g,  const Tag2 &tag);
    void add_body(Graph *g, const Body &body);

    // Copy all vertices specified in srcfacs from the src graph
    // to the destination graph. Also copy all values that are used by
    // those factors, and pin them down with an absolute pose prior to
    // be their current values.
    void copy_subgraph(Graph *dest, const Graph &src,
                       const std::deque<VertexDesc> &srcfacs);
    // Look through the source graph for any values that are not yet
    // optimized in the destination graph. Initialize those values
    // in the destination graph, and add them to the optimizer.
    // Also add any destination graph factors to the optimizer if
    // they can now be optimized
    void initialize_from(Graph *destg, const Graph &srcg);

    // convenience functions for retrieval of optimized poses
    bool get_optimized_pose(const Graph &g, const ros::Time &t,
                            const string &name, Transform *tf);
    bool get_optimized_pose(const Graph &g, const Camera2 &cam, Transform *tf);
    bool get_optimized_pose(const Graph &g, const ros::Time &t,
                            const Body &body, Transform *tf);
    bool get_optimized_pose(const Graph &g, const Tag2 &tag, Transform *tf);

    PoseWithNoise get_optimized_camera_pose(const Graph &g,
                                            const Camera2 &cam);

  }
}
