#include "git_text_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace git_commit_sha {

static const char *const TAG = "git_commit_sha.text_sensor";

void GitTextSensor::setup() {
  this->publish_state(this->git_commit_sha_);
}
float GitTextSensor::get_setup_priority() const { return setup_priority::DATA; }
std::string GitTextSensor::unique_id() { return get_mac_address() + "-git-commit-sha"; }
void GitTextSensor::dump_config() { LOG_TEXT_SENSOR("", "Git Text Sensor", this); }

}  // namespace version
}  // namespace esphome
