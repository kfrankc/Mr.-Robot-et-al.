#include "server_config.h"

#include <iostream>
#include <string>

ServerConfig::ServerConfig(NginxConfig config) {
  fillOutMap(config);
}

ServerConfig::~ServerConfig() {
  // TODO
}

void ServerConfig::fillOutMap(NginxConfig config) {
  // Initialize variables property, value
  std::string property = "";
  std::string value = "";
  for (size_t i = 0; i < config.statements_.size(); i++) {
    //search in child block
    if (config.statements_[i]->child_block_ != nullptr) {
      fillOutMap(*(config.statements_[i]->child_block_));
    }

    if (config.statements_[i]->tokens_.size() >= 1) {
      property = config.statements_[i]->tokens_[0];
    }

    if (config.statements_[i]->tokens_.size() >= 2) {
      value = config.statements_[i]->tokens_[1];
    }

    if (property == "listen" && value != "") {
      property_to_values_["port"] = value;
    }
  }
}

void ServerConfig::printPropertiesMap() {
  // Iterate over an unordered_map using range based for loop
  std::cout << "Mappings in property_to_values_:\n" << std::endl;
  for (std::pair<std::string, std::string> element : property_to_values_) {
    std::cout << element.first << " :: " << element.second << std::endl;
  }
}

std::string ServerConfig::propertyLookUp(std::string propertyName) {
  // unordered_map::at(key) throws if key not found.
  // unordered_map::operator[key] will silently create that entry
  // See: http://www.cplusplus.com/reference/unordered_map/unordered_map/operator[]/
  // and: http://www.cplusplus.com/reference/stdexcept/out_of_range/
  try {
    return property_to_values_.at(propertyName);
  }
  catch (const std::out_of_range& oor) {
    return "";
  }
}