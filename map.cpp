/*
 * Draft implementation for map to store robot trajectories 
 */

#include <map>
#include <vector>
#include <iostream>

#include <Eigen/Geometry>

using RobotStateType = std::vector<double>;
using RobotTrajectoryType = std::vector<RobotStateType>;

struct Key
{
    std::string name;
    Eigen::Isometry3d goal_pose;
    RobotStateType initial_state;

};

uint8_t in_ep (double delta, double epsilon) {
    if (delta <= epsilon)
        return 1;
    if (delta > epsilon)
        return 0;
}

bool isPoseInEpsilon (const Eigen::Isometry3d l, const Eigen::Isometry3d r, double t_eps, double r_eps) {
    double dt = (l.translation() - r.translation()).norm();
    
    Eigen::Quaterniond q_left(l.rotation());
    Eigen::Quaterniond q_right(r.rotation());                                                         
                                                                                                                      
    double rs = std::acos(q_left.x()*q_right.x() + q_left.y()*q_right.y() + q_left.z()*q_right.z() + q_left.w()*q_right.w());
    double rs1 = std::acos(q_left.x()*(-q_right.x()) + q_left.y()*(-q_right.y()) + q_left.z()*(-q_right.z()) + q_left.w()*(-q_right.w()));                                                    
    double rot_dist = std::min(rs, rs1);
    
    uint8_t res_t = in_ep(dt, t_eps);
    uint8_t res_q = in_ep(rot_dist, r_eps);

    return (res_t && res_q);
}

int poseCompare (const Eigen::Isometry3d l, const Eigen::Isometry3d r, double t_eps, double r_eps) {

    bool pose_in_epsilon = isPoseInEpsilon(l, r, t_eps, r_eps);

    if (!pose_in_epsilon) {
        if (l.translation().x() != r.translation().x() )
            return l.translation().x() < r.translation().x();
        if (l.translation().y() != r.translation().y())
            return l.translation().y() < r.translation().y();
        if (l.translation().z() != r.translation().z())
            return l.translation().z() < r.translation().z();

        Eigen::Quaterniond ql(l.rotation());
        Eigen::Quaterniond qr(r.rotation());                                                         
 
        if (ql.w() != qr.w())
           return ql.w() <  qr.w();
        if (ql.x() != qr.x())
           return ql.x() <  qr.x();
        if (ql.y() != qr.y())
           return ql.y() <  qr.y();
        if (ql.z() != qr.z())
           return ql.z() <  qr.z();
    } else {
        return -1;
    }
} 

int stateCompare (const RobotStateType& l, const RobotStateType& r, double ep) {
    bool j_in_ep = false;
    
    for (int i = 0; i< 6; i++) {
        j_in_ep = in_ep(std::abs(l[i] - r[i]), ep);
        if (!j_in_ep)
            return l[i] < r[i];
    }

    return -1;
}

class KeyCompare {
    public:                                                                                                       
        KeyCompare(double translation_epsilon = 1e-1, double rotation_epsilon = 1e-1, double robot_state_epsilon = 1e-1) :
            translation_epsilon_(translation_epsilon), rotation_epsilon_(rotation_epsilon), robot_state_epsilon_(robot_state_epsilon) {
                std::cout << "translation_epsilon: " << translation_epsilon_ << std::endl;
                std::cout << "rotation_epsilon: " << rotation_epsilon_ << std::endl;                                          
                std::cout << "robot_state_epsilon: " << robot_state_epsilon_ << std::endl;                                   
            }                                                                                                               
        bool operator()(const Key& l, const Key& r) const {
            bool name_eq = l.name == r.name;
            bool name_less = l.name < r.name;

            if (!name_eq) {
                std::cout << "Name not equal (" << l.name << "!=" << r.name <<"); Compare by name\n"; 
                return name_less;
            }

            int pose_compare = poseCompare(l.goal_pose, r.goal_pose, translation_epsilon_, rotation_epsilon_ );
            std::cout << "pose_compare: " << pose_compare << std::endl;
            if (pose_compare != -1) {
                std::cout << "Pose not in epsilon; Compare pose lexicographic\n";
                return pose_compare;
            }

            int state_compare = stateCompare(l.initial_state, r.initial_state, robot_state_epsilon_);
            std::cout << "state_compare: " << state_compare << std::endl; 

            if (state_compare != -1) {
                std::cout << "State is not in epsilon; Compare state lexicographic\n";
                return state_compare;
            }

            std::cout << "Key is equal; so Less() return false\n";
            return false;

        };
    private:
        double translation_epsilon_;                                                                               
        double rotation_epsilon_;
        double robot_state_epsilon_;
};

int main () {
    Eigen::Isometry3d p = Eigen::Isometry3d::Identity();
    Eigen::Isometry3d p1 = Eigen::Isometry3d::Identity();
    p1.translation() = Eigen::Vector3d(1, 1, 1);


    RobotTrajectoryType t1 = {{0,0,0,0,0,0}, {1,0,0,0,0,0}};
    Key k1 = {"rname", p, {0,0,0,0,0,0}};
    Key k2 = {"rname2", p1, {1,0,0,0,0,0}};


    std::map<Key, RobotTrajectoryType, KeyCompare> m = {
        {k1, t1},
        {k2, t1}
    };

    const auto& k = m.find(k2);
    std::cout << "Key: [" << k->first.name << "] Value: [" \
        << k->second[0][0] << ", " << k->second[0][1] << ", " << k->second[0][2] << "]\n";

    return 0;
}
