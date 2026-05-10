#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>

using json = nlohmann::json;

class MotionPlayer : public rclcpp::Node
{
public:
    MotionPlayer()
        : Node("motion_player"),
          current_frame_(0),
          is_playing_(false)
    {
        // Publisher
        joint_pub_ =
            this->create_publisher<sensor_msgs::msg::JointState>(
                "/joint_states",
                10);

        // Load motion JSON
        load_motion_file(
          "/home/alesurankar/ros2_ws/walker_s2_motion.json");

        // Timer (30 Hz)
        timer_ =
            this->create_wall_timer(
                std::chrono::milliseconds(33),
                std::bind(&MotionPlayer::playback_callback, this));

        RCLCPP_INFO(
            this->get_logger(),
            "Motion player loaded %zu frames",
            motion_data_.frames.size());
    }

    void start_playback()
    {
        if (motion_data_.frames.empty()) {
            RCLCPP_ERROR(this->get_logger(), "No motion frames loaded");
            return;
        }

        is_playing_ = true;
        current_frame_ = 0;
        start_time_ = std::chrono::steady_clock::now();

        RCLCPP_INFO(
            this->get_logger(),
            "Starting motion playback...");
    }

private:

    // =========================
    // Motion Structures
    // =========================

    struct MotionFrame
    {
        double timestamp;
        int frame_id;
        std::map<std::string, double> joints;
    };

    struct MotionData
    {
        int total_frames;
        double duration;
        std::vector<MotionFrame> frames;
    };

    // =========================
    // Load JSON Motion File
    // =========================

    void load_motion_file(const std::string& filename)
    {
        std::ifstream file(filename);

        if (!file.is_open())
        {
            RCLCPP_ERROR(
                this->get_logger(),
                "Failed to open motion file: %s",
                filename.c_str());
            return;
        }

        json j;

        try
        {
            file >> j;

            motion_data_.total_frames =
                j["total_frames"];

            motion_data_.duration =
                j["duration"];

            for (const auto& frame_json : j["frames"])
            {
                MotionFrame frame;

                frame.timestamp =
                    frame_json["timestamp"];

                frame.frame_id =
                    frame_json["frame_id"];

                // Read joints
                for (auto it = frame_json["joints"].begin();
                     it != frame_json["joints"].end();
                     ++it)
                {
                    frame.joints[it.key()] =
                        it.value();
                }

                motion_data_.frames.push_back(frame);
            }

            RCLCPP_INFO(
                this->get_logger(),
                "Loaded %zu frames from JSON",
                motion_data_.frames.size());
        }
        catch (const std::exception& e)
        {
            RCLCPP_ERROR(
                this->get_logger(),
                "JSON parse error: %s",
                e.what());
        }
    }

    // =========================
    // Playback Timer Callback
    // =========================

    void playback_callback()
    {
        if (!is_playing_ || motion_data_.frames.size() < 2)
            return;

        double elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start_time_
            ).count() / 1000.0;

        // clamp to last segment
        if (elapsed >= motion_data_.frames.back().timestamp)
        {
            publish_joint_state(motion_data_.frames.back());
            return;
        }

        size_t i = 0;

        // find segment: frame[i] → frame[i+1]
        while (i + 1 < motion_data_.frames.size() &&
              elapsed > motion_data_.frames[i + 1].timestamp)
        {
            i++;
        }

        const auto& f0 = motion_data_.frames[i];
        const auto& f1 = motion_data_.frames[i + 1];

        double segment_duration = f1.timestamp - f0.timestamp;

        double t = (elapsed - f0.timestamp) / segment_duration;

        MotionFrame interp = f0;

        for (auto& [joint, value] : interp.joints)
        {
            double v0 = f0.joints.at(joint);
            double v1 = f1.joints.at(joint);

            interp.joints[joint] = v0 + t * (v1 - v0);
        }

        publish_joint_state(interp);
    }

    // =========================
    // Publish Joint State
    // =========================

    void publish_joint_state(const MotionFrame& frame)
    {
        sensor_msgs::msg::JointState msg;

        msg.header.stamp =
            this->get_clock()->now();

        for (const auto& joint : frame.joints)
        {
            msg.name.push_back(joint.first);
            msg.position.push_back(joint.second);
        }

        // Optional arrays
        msg.velocity.resize(msg.name.size(), 0.0);
        msg.effort.resize(msg.name.size(), 0.0);

        joint_pub_->publish(msg);
    }

    // =========================
    // Members
    // =========================

    MotionData motion_data_;

    size_t current_frame_;

    bool is_playing_;

    std::chrono::steady_clock::time_point start_time_;

    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr
        joint_pub_;

    rclcpp::TimerBase::SharedPtr
        timer_;
};

// =========================
// Main
// =========================

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);

    auto player =
        std::make_shared<MotionPlayer>();

    player->start_playback();

    rclcpp::spin(player);

    rclcpp::shutdown();

    return 0;
}