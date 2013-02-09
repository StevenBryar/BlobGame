#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H
//class stores messages
//sends them when there time
//expires.
//update will reduce time on messages
//using delta.check if any messages
//are at zero.
//create message method takes 
//message type,sender,receiver,extra info
//and time to send.
//
#include <vector>

class MessageListener;

struct Message{
	int type;
	MessageListener* sender;
	MessageListener* reciever;
	void* extraInfo;
	double timeToSend;
	std::string name;
	Message(MessageListener* sender,MessageListener* reciever,
			void* extraInfo,double timeToSend,int type,std::string name){
					this->sender = sender;
					this->reciever = reciever;
					this->extraInfo = extraInfo;
					this->timeToSend = timeToSend;
					this->type = type;
					this->name = name;
	}
};

class MessageHandler{
public:
	static MessageHandler* Instance();
	void cleanUpInstance();
	void createMessage(int type,MessageListener* sender,MessageListener* reciever,void* extraInfo,double timeToSend);
	void createMessage(int type,MessageListener* sender,MessageListener* reciever,void* extraInfo,double timeToSend,std::string name);
	void createMessage(int type,MessageListener* sender,MessageListener* recievers[],void* extraInfo,double timeToSend,int numOfRecievers);
	void update();
	void sendMessage(Message* msg);
	void deleteMessage(std::string name);
	void deleteMessage(Message* message);
private:
	static MessageHandler* m_Instance;
	std::vector<Message*>* m_Messages;
	std::vector<Message*>* m_MessagesToDelete;
	MessageHandler();
	~MessageHandler();
};
#endif