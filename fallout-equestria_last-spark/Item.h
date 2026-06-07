#pragma once
#include <string>

class Player;

class Item {
 public:
  Item(const std::string& name, float weight, int value);
  virtual ~Item() = default;

  const std::string& getName() const { return name_it; };
  float getWeight() const { return weight_it; };
  int getValue() const { return value_it; };
  int getSellValue() const;
  virtual void use(Player* user);

 protected:
  std::string name_it;
  float weight_it;
  int value_it;
};
