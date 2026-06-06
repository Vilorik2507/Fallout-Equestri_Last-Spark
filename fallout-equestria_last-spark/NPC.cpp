#include "NPC.h"

#include <iostream>

#include "Game.h"
#include "Item.h"
#include "Player.h"

NPC::NPC(const std::string& name, const Dialogue* dialogue,
         const Faction* faction)
    : name_npc(name), dialogue(dialogue), faction_npc(faction) {};

void NPC::talkWithPlayer() {
  if (!dialogue) {
    std::cout << name_npc << " has nothing to say." << std::endl;
    return;
  }

  DialogueNode* currentNode = dialogue->getStartNode();
  if (!currentNode) {
    std::cout << name_npc << " seems lost for words." << std::endl;
    return;
  }

  std::shared_ptr<Player> player = Game::getPlayer();

  if (!player) {
    std::cout << "Error: Player not found!" << std::endl;
    return;
  }

  while (currentNode) {
    std::cout << name_npc << ": " << currentNode->getNpcLine() << std::endl;

    const std::vector<Choice>& choices = currentNode->getChoices();

    if (choices.empty()) {
      break;
    }

    for (size_t i = 0; i < choices.size(); ++i) {
      std::cout << i + 1 << ". " << choices[i].getText() << std::endl;
    }

    if (hasShop()) {
      std::cout << choices.size() + 1 << ". Торговля" << std::endl;
    }

    int choiceIndex;
    std::cout << "Your choice: ";
    std::cin >> choiceIndex;

    if (hasShop() && choiceIndex == static_cast<int>(choices.size()) + 1) {
      trade();
      continue;
    }

    if (choiceIndex < 1 || choiceIndex > static_cast<int>(choices.size())) {
      std::cout << "Invalid choice. Please try again." << std::endl;
      continue;
    }

    const Choice& selected = choices[choiceIndex - 1];

    const std::map<Faction*, int>& repDelta = selected.getReputationDelta();
    for (const auto& [faction, delta] : repDelta) {
      if (faction) {
        player->modifyReputation(*faction, delta);
      }
    }

    const std::string& nextNodeId = selected.getNextNodeId();
    if (nextNodeId.empty() || nextNodeId == "end") {
      break;
    }

    currentNode = dialogue->getNode(nextNodeId);
    if (!currentNode) {
      std::cout << "Error: dialogue node '" << nextNodeId << "' not found."
                << std::endl;
      break;
    }
  }

  std::cout << "Dialogue ended." << std::endl;
}

void NPC::trade() {
  auto player = Game::getPlayer();
  if (!player) {
    std::cout << "Error: Player not found!" << std::endl;
    return;
  }

  while (true) {
    std::cout << "\n--- " << name_npc << "'s Shop ---" << std::endl;
    std::cout << "Your gold: " << player->getGold() << std::endl;
    std::cout << "1. Buy" << std::endl;
    std::cout << "2. Sell" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choice: ";

    int mainChoice;
    std::cin >> mainChoice;

    if (mainChoice == 0) break;

    if (mainChoice == 1) {
      // Покупка
      while (true) {
        std::cout << "\n--- Buy from " << name_npc << " ---" << std::endl;
        std::cout << "Your gold: " << player->getGold() << std::endl;
        if (shop_items_npc.empty()) {
          std::cout << "No items for sale." << std::endl;
          break;
        }
        for (size_t i = 0; i < shop_items_npc.size(); ++i) {
          std::cout << i + 1 << ". " << shop_items_npc[i]->getName()
                    << " - Price: " << shop_items_npc[i]->getValue() << " gold"
                    << std::endl;
        }
        std::cout << "0. Back" << std::endl;
        std::cout << "Choice: ";

        int buyChoice;
        std::cin >> buyChoice;
        if (buyChoice == 0) break;
        if (buyChoice < 1 ||
            buyChoice > static_cast<int>(shop_items_npc.size())) {
          std::cout << "Invalid choice." << std::endl;
          continue;
        }

        auto& item = shop_items_npc[buyChoice - 1];
        if (player->getGold() >= item->getValue()) {
          player->spendGold(item->getValue());
          player->addItem(
              item);  // предполагается, что addItem принимает shared_ptr<Item>
          std::cout << "You bought " << item->getName() << "!" << std::endl;
        } else {
          std::cout << "Not enough gold!" << std::endl;
        }
      }
    } else if (mainChoice == 2) {
      // Продажа
      while (true) {
        std::cout << "\n--- Sell to " << name_npc << " ---" << std::endl;
        std::cout << "Your gold: " << player->getGold() << std::endl;
        auto& inventory = player->getInventory();
        const auto& playerItems = inventory.getItems();
        if (playerItems.empty()) {
          std::cout << "You have nothing to sell." << std::endl;
          break;
        }
        for (size_t i = 0; i < playerItems.size(); ++i) {
          std::cout << i + 1 << ". " << playerItems[i]->getName()
                    << " - Sell price: " << playerItems[i]->getSellValue()
                    << " gold" << std::endl;
        }
        std::cout << "0. Back" << std::endl;
        std::cout << "Choice: ";

        int sellChoice;
        std::cin >> sellChoice;
        if (sellChoice == 0) break;
        if (sellChoice < 1 ||
            sellChoice > static_cast<int>(playerItems.size())) {
          std::cout << "Invalid choice." << std::endl;
          continue;
        }

        auto& item = playerItems[sellChoice - 1];
        int sellPrice = item->getSellValue();
        player->addGold(sellPrice);
        inventory.removeItem(item);
        std::cout << "You sold " << item->getName() << " for " << sellPrice
                  << " gold!" << std::endl;
      }
    } else {
      std::cout << "Invalid choice." << std::endl;
    }
  }
}

void NPC::addShopItem(std::shared_ptr<Item> item) {
  if (item) shop_items_npc.push_back(item);
}

bool NPC::hasShop() const { return !shop_items_npc.empty(); }
