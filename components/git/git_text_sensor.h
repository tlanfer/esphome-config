#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"

#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace git_commit_sha {

class GitTextSensor : public text_sensor::TextSensor, public Component {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  std::string unique_id() override;

  void set_git_commit_sha(std::string git_commit_sha) { this->git_commit_sha_ = git_commit_sha; };

 protected:
  std::string git_commit_sha_{""};
};

}  // namespace git
}  // namespace esphome
