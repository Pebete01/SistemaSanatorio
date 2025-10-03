#pragma once
#include <string>
#include <vector>

void init_ui();
void shutdown_ui();
void message_center(const std::string &title, const std::string &text);

int run_menu(const std::vector<std::string> &items);
int run_menu_titled(const std::string &title, const std::vector<std::string> &items);
int run_submenu(const std::string &title, const std::vector<std::string> &items);

std::string input_box(const std::string &title, const std::string &prompt, int maxlen);
bool confirm_box(const std::string &title, const std::string &question);

void list_box(const std::string &title, const std::vector<std::string> &lines);
