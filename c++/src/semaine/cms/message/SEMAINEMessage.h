/*
 *  SEMAINEMessage.h
 *  semaine
 *
 *  Created by Marc Schröder on 09.09.08.
 *  Copyright 2008 DFKI GmbH. All rights reserved.
 *
 */


#ifndef SEMAINE_CMS_MESSAGE_SEMAINEMESSAGE_H
#define SEMAINE_CMS_MESSAGE_SEMAINEMESSAGE_H

#include <typeinfo>

#include <semaine/config.h>

#include <semaine/cms/exceptions/MessageFormatException.h>

#include <cms/Message.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/Destination.h>
#include <cms/Topic.h>

using namespace cms;

namespace semaine {
namespace cms {
namespace message {

/**
 * A representation of a message as used in SEMAINE code.
 * It is a higher-level access method for JMS messages,
 * providing interpretations of information accessible in the message headers.
 *
 * This base class provides access to the Message body only as plain text
 * (for text messages). Subclasses should:
 * <ul>
 * <li>add any specific message format verification code in their constructor;</li>
 * <li>provide their own API for accessing the message content.</li>
 * </ul>
 *
 * @author marc
 *
 */
class SEMAINEMessage
{
public:
	/**
	 * The name of the property used for time in "user" time that the message refers to.
	 */
	static const std::string USERTIME;
	/**
	 * The name of the property used for the type of data being sent.
	 */
	static const std::string DATATYPE;
	/**
	 * The name of the property used for the component that sent the data.
	 */
	static const std::string SOURCE;
	/**
	 * The name of the property used for the type of event being sent (for event-based messages).
	 */
	static const std::string EVENT;
	/**
	 * The name of the property used for the period length, in milliseconds (for periodic messages).
	 */
	static const std::string PERIOD;

	/**
	 * The name of the property used for the content's unique identifier, if any.
	 */
	static const std::string CONTENT_ID;

	/**
	 * The name of the property used for the content's creation time, if given.
	 */
	static const std::string CONTENT_CREATION_TIME;

	/**
	 * The name of the property used for the content's type, if any.
	 */
	static const std::string CONTENT_TYPE;

	/**
	 * A possible value of the CONTENT_TYPE property, representing a speaker utterance.
	 */
	static const std::string CONTENT_TYPE_UTTERANCE;

	/**
	 * A possible value of the CONTENT_TYPE property, representing a listener vocalisation.
	 */
	static const std::string CONTENT_TYPE_LISTENERVOCALISATION;

	/**
	 * A possible value of the CONTENT_TYPE property, representing a visual-only agent behaviour.
	 */
	static const std::string CONTENT_TYPE_VISUALONLY;

	/**
	 * Create a SEMAINE specific abstraction from the given message.
	 * @param message a message containing SEMAINE-specific properties.
	 * @throws NullPointerException if message is null.
	 * @throws MessageFormatException if the message does not have the proper format
	 */
	SEMAINEMessage(const Message * message) throw(semaine::cms::exceptions::MessageFormatException);

	virtual ~SEMAINEMessage();

	/**
	 * Get the user time that the message refers to, in milliseconds since 1970.
	 * @return the user time
	 * @throws CMSException if the CMS provider fails to get the property value due to some internal error.
	 */
	long long getUsertime() throw(CMSException)
	{
		return message->getLongProperty(USERTIME);
	}

	/**
	 * Get the name of the data type received.
	 * @return a string containing the name of the data type
	 * @throws CMSException if the CMS provider fails to get the property value due to some internal error.
	 */
	std::string getDatatype() throw(CMSException)
	{
		return message->getStringProperty(DATATYPE);
	}

	/**
	 * Get the name of the source component that sent the data.
	 * @return a string containing the name of the source component
	 * @throws CMSException if the CMS provider fails to get the property value due to some internal error.
	 */
	std::string getSource() throw(CMSException)
	{
		return message->getStringProperty(SOURCE);
	}

	/**
	 * Determine whether the message is periodic. This is determined
	 * by the presence of the <code>period</code> property.
	 * @return <code>true</code> if message is periodic,
	 * <code>false</code> if message is event-based.
	 * @throws CMSException if the CMS provider fails to get the property value due to some internal error.
	 */
	bool isPeriodic() throw(CMSException)
	{
		return message->propertyExists(PERIOD);
	}

	/**
	 * Determine whether the message is event-based. This is determined
	 * by the presence of the <code>event</code> property.
	 * @return <code>true</code> if message is event-based,
	 * <code>false</code> if message is periodic.
	 * @throws CMSException if the CMS provider fails to get the property value due to some internal error.
	 */
	bool isEventBased() throw(CMSException)
	{
		return message->propertyExists(EVENT);
	}

	/**
	 * For event-based messages, determine the event type.
	 * @return an Event object representing the event type.
	 * @throws CMSException if the CMS provider fails to get the property value due to some internal error.
	 * @throws MessageFormatException if the message contains an <code>event</code> property, but the value is not known,
	 * or if the message is not event-based.
	 */
	std::string getEventType() throw (CMSException, semaine::cms::exceptions::MessageFormatException)
	{
		if (!message->propertyExists(EVENT))
			throw semaine::cms::exceptions::MessageFormatException("Message is not event-based, cannot provide event type");
		std::string eventString = message->getStringProperty(EVENT);
		return eventString;
	}

	/**
	 * For periodic messages, determine the period length.
	 * @return the period length, in milliseconds.
	 * @throws CMSException if the JMS provider fails to get the property value due to some internal error.
	 * @throws MessageFormatException if the message is not periodic.
	 */
	int getPeriod() throw (CMSException, semaine::cms::exceptions::MessageFormatException)
	{
		if (!message->propertyExists(PERIOD))
			throw semaine::cms::exceptions::MessageFormatException("Message is not periodic, cannot provide period length");
		return message->getIntProperty(PERIOD);
	}

	/**
	 * Get the unique ID of the content in the message, if any.
	 * @return the content ID as a string, or the empty string if the message doesn't have a content ID.
	 * @throws CMSException if the CMS provider fails to get the property value due to some internal error.
	 */
	std::string getContentID() throw (CMSException)
	{
		if (message->propertyExists(CONTENT_ID)) {
			return message->getStringProperty(CONTENT_ID);
		}
		return "";
	}

	/**
	 * Get the type of the content in the message, if any.
	 * @return the content type as a string, or the empty string if the message doesn't have a content type.
	 * The value may be one of CONTENT_TYPE_UTTERANCE, CONTENT_TYPE_LISTENERVOCALISATION, CONTENT_TYPE_VISUALONLY, or any other string,
	 * or the empty string if the message does not have this field.
	 * @throws CMSException if the CMS provider fails to get the property value due to some internal error.
	 */
	std::string getContentType() throw (CMSException)
	{
		if (message->propertyExists(CONTENT_TYPE)) {
			return message->getStringProperty(CONTENT_TYPE);
		}
		return "";
	}

	/**
	 * Get the time when the content was originally created.
	 * @return the time in user time, or -1 if the message doesn't contain this property.
	 * @throws CMSException if the CMS provider fails to get the property value due to some internal error.
	 */
	long long getContentCreationTime() throw (CMSException)
	{
		if (message->propertyExists(CONTENT_CREATION_TIME)) {
			return message->getLongProperty(CONTENT_CREATION_TIME);
		}
		return -1;
	}

	/**
	 * Determine whether the message is a text message.
	 * @return true if the message is a text message, false otherwise.
	 */
	bool isTextMessage()
	{
		return dynamic_cast<const TextMessage*>(message) != NULL;
	}

	/**
	 * Determine whether the message is a bytes message.
	 * @return true if the message is a bytes message, false otherwise.
	 */
	bool isBytesMessage()
	{
		return dynamic_cast<const BytesMessage*>(message) != NULL;
	}

	/**
	 * For text messages, provide access to the text sent.
	 * @return the text sent in the body of the message.
	 * @throws CMSException if the JMS provider fails to get the property value due to some internal error.
	 * @throws MessageFormatException if the message is not a text message.
	 */
	std::string getText() throw(CMSException, semaine::cms::exceptions::MessageFormatException)
	{
		const TextMessage * tm = dynamic_cast<const TextMessage*>(message);
		if (tm == NULL) {
			throw semaine::cms::exceptions::MessageFormatException(std::string("Cannot get text for a message of type ")+typeid(*message).name());
		}
		return tm->getText();
	}

	std::string getTopicName() throw(CMSException)
	{
		const Destination * dest = message->getCMSDestination();
		if (dest == NULL) return std::string("unknown");
		const Topic * topic = dynamic_cast<const Topic *>(dest);
		if (topic == NULL) return std::string("unknown");
		return topic->getTopicName();
	}

	/**
	 * Provide access to the low-level message encapsulated in this object.
	 * @return the low-level message object.
	 */
	const Message * getMessage()
	{
		return message;
	}




protected:
	const Message * message;

private:
		/**
	 * Check that the message conforms to our expectations.
	 * This method is private because overriding it makes no sense:
	 * as it is called from the constructor, the version in the subclass
	 * would not be visible from our constructor.
	 * @throws MessageFormatException if a problem with the properties is found.
	 */
	void verifyProperties() throw(semaine::cms::exceptions::MessageFormatException);



};


} // namespace message
} // namespace cms
} // namespace semaine



#endif

