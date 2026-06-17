#include "CSVHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// TODO: Implement CSV I/O functions
void CSVHandler::loadUsers(const std::string& filename, Graph& graph) {}
void CSVHandler::loadFriendships(const std::string& filename, Graph& graph) {}
void CSVHandler::saveUsers(const std::string& filename, const Graph& graph) {}
void CSVHandler::saveFriendships(const std::string& filename, const Graph& graph) {}
void CSVHandler::appendUser(const std::string& filename, const User& user) {}
void CSVHandler::appendFriendship(const std::string& filename, int u, int v) {}
void CSVHandler::rewriteUsers(const std::string& filename, const Graph& graph) {}
void CSVHandler::rewriteFriendships(const std::string& filename, const Graph& graph) {}
