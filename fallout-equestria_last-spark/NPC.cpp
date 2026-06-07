#include "NPC.h"

#include <iostream>

#include "Game.h"
#include "Item.h"
#include "Player.h"
#include "Printer.h"

NPC::NPC(const std::string& name, const Dialogue* dialogue,
         const Faction* faction)
    : name_npc(name), dialogue(dialogue), faction_npc(faction) {};

void NPC::talkWithPlayer() {
  if (!dialogue) {
    slow_cout << name_npc << " нечего сказать." << std::endl;
    return;
  }

  DialogueNode* currentNode = dialogue->getStartNode();
  if (!currentNode) {
    slow_cout << name_npc << " тупит." << std::endl;
    return;
  }

  std::shared_ptr<Player> player = Game::getPlayer();

  if (!player) {
    slow_cout << "Error: Player not found!" << std::endl;
    return;
  }

  while (currentNode) {
    slow_cout.setMode(SlowMode::CharByChar);
    slow_cout.setDelay(25);
    slow_cout << name_npc << ": " << currentNode->getNpcLine() << std::endl;
    slow_cout.setDelay(200);
    slow_cout.setMode(SlowMode::LineByLine);
    const std::vector<Choice>& choices = currentNode->getChoices();

    if (choices.empty()) {
      break;
    }

    for (size_t i = 0; i < choices.size(); ++i) {
      slow_cout << i + 1 << ". " << choices[i].getText() << std::endl;
    }

    if (hasShop()) {
      slow_cout << choices.size() + 1 << ". Торговля" << std::endl;
    }

    int choiceIndex;
    slow_cout << "Выбор: ";
    std::cin >> choiceIndex;

    if (hasShop() && choiceIndex == static_cast<int>(choices.size()) + 1) {
      trade();
      continue;
    }

    if (choiceIndex < 1 || choiceIndex > static_cast<int>(choices.size())) {
      slow_cout << "Неверный выбор." << std::endl;
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
      slow_cout << "Error: dialogue node '" << nextNodeId << "' not found."
                << std::endl;
      break;
    }
  }

  slow_cout << "Диалог закончен." << std::endl;
}

void NPC::trade() {
  auto player = Game::getPlayer();
  if (!player) {
    slow_cout << "Error: Player not found!" << std::endl;
    return;
  }

  while (true) {
    slow_cout << "\n--- " << name_npc << "'s Торговля ---" << std::endl;
    slow_cout << "Your gold: " << player->getGold() << std::endl;
    slow_cout << "1. Купить" << std::endl;
    slow_cout << "2. Продать" << std::endl;
    slow_cout << "0. Выход" << std::endl;
    slow_cout << "Выбор: ";

    int mainChoice;
    std::cin >> mainChoice;

    if (mainChoice == 0) break;

    if (mainChoice == 1) {
      // Покупка
      while (true) {
        slow_cout << "\n--- Купить у " << name_npc << " ---" << std::endl;
        slow_cout << "Ваши крышки: " << player->getGold() << std::endl;
        if (shop_items_npc.empty()) {
          slow_cout << "Нет вещей на продажу." << std::endl;
          break;
        }
        for (size_t i = 0; i < shop_items_npc.size(); ++i) {
          slow_cout << i + 1 << ". " << shop_items_npc[i]->getName()
                    << " - Цена: " << shop_items_npc[i]->getValue() << " крышек"
                    << std::endl;
        }
        slow_cout << "0. Назад" << std::endl;
        slow_cout << "Выбор: ";

        int buyChoice;
        std::cin >> buyChoice;
        if (buyChoice == 0) break;
        if (buyChoice < 1 ||
            buyChoice > static_cast<int>(shop_items_npc.size())) {
          slow_cout << "Неверный выбор." << std::endl;
          continue;
        }

        auto& item = shop_items_npc[buyChoice - 1];
        if (player->getGold() >= item->getValue()) {
          player->spendGold(item->getValue());
          player->addItem(item);
          slow_cout << "Вы покупаете " << item->getName() << "!" << std::endl;
        } else {
          slow_cout << "Недостаточно крышек!" << std::endl;
        }
      }
    } else if (mainChoice == 2) {
      // Продажа
      while (true) {
        slow_cout << "\n--- Продать " << name_npc << " ---" << std::endl;
        slow_cout << "Ваши крышки: " << player->getGold() << std::endl;
        auto& inventory = player->getInventory();
        const auto& playerItems = inventory.getItems();
        if (playerItems.empty()) {
          slow_cout << "Нечего продавать." << std::endl;
          break;
        }
        for (size_t i = 0; i < playerItems.size(); ++i) {
          slow_cout << i + 1 << ". " << playerItems[i]->getName()
                    << " - Цена продажи: " << playerItems[i]->getSellValue()
                    << " крышек" << std::endl;
        }
        slow_cout << "0. Назад" << std::endl;
        slow_cout << "Выбор: ";

        int sellChoice;
        std::cin >> sellChoice;
        if (sellChoice == 0) break;
        if (sellChoice < 1 ||
            sellChoice > static_cast<int>(playerItems.size())) {
          slow_cout << "Неверный выбор." << std::endl;
          continue;
        }

        auto& item = playerItems[sellChoice - 1];
        int sellPrice = item->getSellValue();
        player->addGold(sellPrice);
        inventory.removeItem(item);
        slow_cout << "Вы продали " << item->getName() << " за " << sellPrice
                  << " крышек!" << std::endl;
      }
    } else {
      slow_cout << "Неверный выбор." << std::endl;
    }
  }
}

void NPC::addShopItem(std::shared_ptr<Item> item) {
  if (item) shop_items_npc.push_back(item);
}

bool NPC::hasShop() const { return !shop_items_npc.empty(); }
