#include <functional>
#include <iostream>
#include <string>
#include<vector>
#include <unordered_map>

#include <Eigen/Geometry>

using RobotStateType = std::vector<double>;
using RobotTrajectoryType = std::vector<RobotStateType>;

struct Key
{
    std::string name;
    Eigen::Isometry3d goal_pose;
    RobotStateType initial_state;

    bool operator==(const Key &other) const {
        return name == other.name;
    }

};

#if 0
// http://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
template <class T>
inline void hash_combine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}
#endif

// Hash function for Eigen vector.
// Based on here: https://wjngkoh.wordpress.com/2015/03/04/c-hash-function-for-eigen-matrix-and-vector/
template<typename _Scalar, int _Rows>
struct std::hash<Eigen::Matrix<_Scalar, _Rows, 1>> {
    std::size_t operator() (const Eigen::Matrix<_Scalar, _Rows, 1>& vector) const {
        std::size_t hash = 0;
        for (ssize_t idx = 0; idx < vector.size(); idx++) {
//            std::hash_combine(hash, vector(idx));
            hash ^= std::hash<double>()(vector(idx)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

template<typename _Scalar, int _Rows, int _Cols>
struct std::hash<Eigen::Matrix<_Scalar, _Rows, _Cols>> {
    std::size_t operator() (const Eigen::Matrix<_Scalar, _Rows, _Cols>& matrix) const {
        std::size_t hash = 0;
        for (ssize_t idx = 0; idx < matrix.size(); idx++) {
//            std::hash_combine(hash, vector(idx));
            auto elem = *(matrix.data() + idx);
            hash ^= std::hash<_Scalar>()(matrix(idx)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};


// Use function as in boost 
// https://www.boost.org/doc/libs/1_72_0/doc/html/hash/reference.html#boost.hash_combine
template<>
struct std::hash<std::vector<double>> {
    std::size_t operator()(std::vector<double> const& vec) const {
        std::size_t seed = vec.size();
        for(auto& i : vec) {
            seed ^= std::hash<double>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
}; 

template<>
struct std::hash<Key> {
    std::size_t operator()(const Key &k) const {
#if 0
        std::size_t seed = 0;
        hash_combine(seed, k.name);
        hash_combine(seed, k.goal_pose);
        hash_combine(seed, k.initial_state);
#endif
        return ((std::hash<string>()(k.name) 
                ^ (std::hash<Eigen::Matrix<double, 4, 4>>()(k.goal_pose.matrix()) << 1 )) >> 1)
                ^ (std::hash<std::vector<double>> ()(k.initial_state) << 1) ;
    }
};

int main()
{
    Eigen::Isometry3d p = Eigen::Isometry3d::Identity();
    RobotTrajectoryType t1 = {{0,0,0,0,0,0}, {1,0,0,0,0,0} };
    Key k1 = {"rname", p, {0,0,0,0,0,0}};
    Key k2 = {"rname2", p, {1,0,0,0,0,0}};
    std::unordered_map<Key, RobotTrajectoryType> um = {
        {k1, t1},
        {k2, t1}
    };

    for (const auto& n: um) {
        std::cout << "Key: [" << n.first.name <<"] Value: [" \
            << n.second[0][0] << "," << n.second[0][1] << "," << n.second[0][2] << "," << n.second[0][3] << "," << n.second[0][4] << "," << n.second[0][5] << "]\n"; 
        std::cout << "\t Hash: " << std::hash<Key>{}(n.first) << std::endl;
    }
    return 0;
}
