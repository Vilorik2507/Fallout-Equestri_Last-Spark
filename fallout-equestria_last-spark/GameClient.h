#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

enum class EventType {
	// Типы событий по вашему усмотрению
};

class GameClient {
public:
	GameClient() = default;
	~GameClient() = default;

	bool connect(const std::string& ip, int port);
	void disconnect();
	void find_match(const std::string& nickname);
	void send_action(const std::string& action);
	void send_chat(const std::string& message);
	void update();
	void register_callback(EventType event, std::function<void(void*)> cb);

private:
	void receive_loop();
	void process_message(const std::string& data);
	void parse_server_message(const std::string& msg);
	void trigger_event(EventType event, void* data);

	int socket;
	struct sockaddr_in* server_addr;
	bool is_connected;
	bool is_in_match;
	std::string player_nickname;
	std::string opponent_name;
	std::thread receive_thread;
	std::queue<std::string> message_queue;
	std::mutex queue_mutex;
	std::map<EventType, std::function<void(void*)>> event_callback;
};
#endif // !GAME_CLIENT_H

