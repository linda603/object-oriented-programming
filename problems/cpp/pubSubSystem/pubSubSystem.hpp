#ifndef PUBSUBSYSTEM_HPP
#define PUBSUBSYSTEM_HPP

#include <string>
#include <vector>
#include <ctime>
#include <unordered_map>

class Message {
private:
    std::string topic;
    std::string content;
    std::time_t timestamp;

public:
    Message(std::string topic, std::string content);

    std::string getTopic() const;
    std::string getContent() const;
};


class Subcriber {
private:
    std::string subcriberId;
    std::string name;
    std::vector<Message*> messageQueue;
    bool active;

public:
    Subcriber(std::string subcriberId, std::string name);

    std::string getId() const;
    std::string getName() const;
    bool isActive() const;
    const std::vector<Message*>& getMessageQueue() const;
    
    void receiveMessage(Message* message);
    void clearMessage();
    void setActive(bool status);
};

class Topic {
private:
    std::string name;
    std::string description;
    std::unordered_map<std::string, Subcriber*> subcribers_map;
    bool active;

public:
    Topic(std::string name, std::string description);

    std::string getName() const;
    std::string getDescription() const;
    bool isActive() const;
    std::vector<Subcriber*> getSubcribers() const;
    void addSubscriber(Subcriber* subcriber);
    void removeSubscriber(Subcriber* subsriber);
    void publishMessage(Message* message);
    void setActive(bool status);
};

class PubSubSystem {
private:
    std::unordered_map<std::string, Topic*> topics_map;
    std::unordered_map<std::string, Subcriber*> subcribers_map;
    int subscriberIdCounter;

public:
    PubSubSystem();
    //~PubSubSystem();

    Topic* createTopic(std::string name, std::string description);
    void removeTopic(std::string name);

    Subcriber* addSubcriber(std::string name);
    bool subscribe(std::string subcriberId, std::string topicName);
    bool publish(std::string topicName, std::string content);

private:
    std::string generateSubcriberId();
    Topic* findTopic(std::string topicName) const;
    Subcriber* findSubcriber(std::string subcriberId) const;
};

#endif