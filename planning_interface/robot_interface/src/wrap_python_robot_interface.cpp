/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2013, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: Ioan Sucan */

#include <moveit/common_planning_interface_objects/common_objects.h>
#include <moveit/py_bindings_tools/roscpp_initializer.h>
#include <moveit/py_bindings_tools/py_conversions.h>

#include <boost/python.hpp>
#include <Python.h>

/** @cond IGNORE */

namespace bp = boost::python;

namespace moveit
{
class RobotInterfacePython : protected py_bindings_tools::ROScppInitializer
{
public:
  RobotInterfacePython(const std::string &robot_description) : 
    py_bindings_tools::ROScppInitializer()
  {
    robot_model_ = planning_interface::getSharedRobotModel(robot_description);
  }
  
  bp::list getJointNames() const
  {
    return py_bindings_tools::listFromString(robot_model_->getJointModelNames());
  }
  
  bp::list getLinkNames() const
  {
    return py_bindings_tools::listFromString(robot_model_->getLinkModelNames());
  }
  
  bp::list getGroupNames() const
  {
    return py_bindings_tools::listFromString(robot_model_->getJointModelGroupNames());
  }
  
  bp::list getJointLimits(const std::string &name) const
  {
    bp::list result;
    const robot_model::JointModel *jm = robot_model_->getJointModel(name);
    if (jm)
    {
      const std::vector<moveit_msgs::JointLimits> &lim = jm->getVariableLimits();
      for (std::size_t i = 0 ; i < lim.size() ; ++i)
      {
        bp::list l;
        l.append(lim[i].min_position);
        l.append(lim[i].max_position);
        result.append(l);
      }
    }
    return result;    
  }
  
  const char* getPlanningFrame() const
  {
    return robot_model_->getModelFrame().c_str();
  }
  
  bp::list getLinkPose() const
  {
  }
  
  bp::dict getCurrentJointValues() const
  {
  }
  
private:
  robot_model::RobotModelConstPtr robot_model_;
};
}

static void wrap_robot_interface()
{
  using namespace moveit;
  
  bp::class_<RobotInterfacePython> RobotClass("RobotInterface", bp::init<std::string>());

  RobotClass.def("get_joint_names", &RobotInterfacePython::getJointNames);
  RobotClass.def("get_group_names", &RobotInterfacePython::getGroupNames);
  RobotClass.def("get_link_names", &RobotInterfacePython::getLinkNames);
  RobotClass.def("get_joint_limits", &RobotInterfacePython::getJointLimits);
  RobotClass.def("get_link_pose", &RobotInterfacePython::getLinkPose);
  RobotClass.def("get_planning_frame", &RobotInterfacePython::getPlanningFrame);
  RobotClass.def("get_current_joint_values", &RobotInterfacePython::getCurrentJointValues);
}

BOOST_PYTHON_MODULE(_moveit_robot_interface)
{
  wrap_robot_interface();
}

/** @endcond */