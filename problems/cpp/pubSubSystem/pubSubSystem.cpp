#include "pubSubSystem.hpp"
#include <iostream>
#include <thread>

Message::Message(std::string topic, std::string content)
    : topic(topic), content(content) {
        timestamp = std::time(nullptr);
}

std::string Message::getTopic() const { return topic; }
std::string Message::getContent() const { return content; }

Subcriber::Subcriber(std::string subcriberId, std::string name)
    : subcriberId(subcriberId), name(name), active(true) {}

std::string Subcriber::getId() const { return subcriberId; }
std::string Subcriber::getName() const { return name; }
bool Subcriber::isActive() const { return active; }
const std::vector<Message*>& Subcriber::getMessageQueue() const { return messageQueue; }

void Subcriber::receiveMessage(Message* message) {
    if (active) {
        messageQueue.push_back(message);
        std::cout << name << " received a new message\n" << std::endl;
    }
}

void Subcriber::clearMessage() {
    messageQueue.clear();
}

void Subcriber::setActive(bool status) {
    active = status;
}

Topic::Topic(std::string name, std::string description)
    : name(name), description(description), active(true) {}

std::string Topic::getName() const { return name; }
std::string Topic::getDescription() const { return description; }
bool Topic::isActive() const { return active; }
std::vector<Subcriber*> Topic::getSubcribers() const {
    std::vector<Subcriber*> res;

    for (auto it = subcribers_map.begin(); it != subcribers_map.end(); it++) {
        res.push_back(it->second);
    }
    return res;
}

void Topic::addSubscriber(Subcriber* subcriber) {
    if (!subcriber) return;
    subcribers_map[subcriber->getId()] = subcriber;
}

void Topic::removeSubscriber(Subcriber* subcriber) {
    if (!subcriber) return;
    auto it = subcribers_map.find(subcriber->getId());
    if (it != subcribers_map.end()) {
        subcribers_map.erase(it);
    }
}

void Topic::publishMessage(Message* message) {
    if (!active) return;

    std::cout << "A new message is broad casting - " << message->getTopic() << std::endl;
    std::cout << "Content: " << message->getContent() << std::endl; 
    for (auto it = subcribers_map.begin(); it != subcribers_map.end(); it++) {
        it->second->receiveMessage(message);
    }
}

void Topic::setActive(bool status) {
    active = status;
}

PubSubSystem::PubSubSystem() : subscriberIdCounter(1) {}

Topic* PubSubSystem::createTopic(std::string name, std::string description) {
    if (findTopic(name)) return nullptr;

    Topic* topic = new Topic(name, description);
    topics_map[name] = topic;
    return topic;
}

void PubSubSystem::removeTopic(std::string name) {
    for (auto it = topics_map.begin(); it != topics_map.end(); it++) {
        if (it->first == name) {
            topics_map.erase(it);
            return;
        }
    }
}

Subcriber* PubSubSystem::addSubcriber(std::string name) {
    std::string subcriberId = generateSubcriberId();
    Subcriber* subcriber = new Subcriber(subcriberId, name);
    subcribers_map[subcriberId] = subcriber;
    return subcriber;
}

std::string PubSubSystem::generateSubcriberId() {
    return "S" + std::to_string(subscriberIdCounter++);
}


bool PubSubSystem::subscribe(std::string subcriberId, std::string topicName) {
    Topic* topic = findTopic(topicName);
    Subcriber* subcriber = findSubcriber(subcriberId);
    if (!topic || !subcriber) return false;

    topic->addSubscriber(subcriber);
    return true;
}

bool PubSubSystem::publish(std::string topicName, std::string content) {
    Topic* topic = findTopic(topicName);
    if (!topic || !topic->isActive()) return false;

    Message* message = new Message(topicName, content);
    topic->publishMessage(message);
    return true;
}

Topic* PubSubSystem::findTopic(std::string topicName) const {
    for (auto it = topics_map.begin(); it != topics_map.end(); it++) {
        if (it->first == topicName) {
            return it->second;
        }
    }
    return nullptr;
}

Subcriber* PubSubSystem::findSubcriber(std::string subcriberId) const {
    for (auto it = subcribers_map.begin(); it != subcribers_map.end(); it++) {
        if (it->first == subcriberId) {
            return it->second;
        }
    }
    return nullptr;
}

int main() {
    PubSubSystem pubSubSystem;

    // Create new topics
    Topic* topic1 = pubSubSystem.createTopic("Topic 1", "Description 1");
    Topic* topic2 = pubSubSystem.createTopic("Topic 2", "Description 2");
    Topic* topic3 = pubSubSystem.createTopic("Topic 3", "Description 3");
   
    // Add subcribers
    Subcriber* sub1 = pubSubSystem.addSubcriber("sub1");
    Subcriber* sub2 = pubSubSystem.addSubcriber("sub2");
    Subcriber* sub3 = pubSubSystem.addSubcriber("sub3");

    // Subcribe to topic
    pubSubSystem.subscribe(sub1->getId(), "Topic 1");
    pubSubSystem.subscribe(sub1->getId(), "Topic 2");
    pubSubSystem.subscribe(sub2->getId(), "Topic 1");
    pubSubSystem.subscribe(sub2->getId(), "Topic 3");
    pubSubSystem.subscribe(sub3->getId(), "Topic 1");

    pubSubSystem.publish("Topic 1", "New AI released today");
    pubSubSystem.publish("Topic 2", "Threads definition");

    return 0;
}