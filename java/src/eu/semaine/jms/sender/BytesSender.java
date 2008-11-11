/**
 * Copyright (C) 2008 DFKI GmbH. All rights reserved.
 * Use is subject to license terms -- see license.txt.
 */
package eu.semaine.jms.sender;

import javax.jms.BytesMessage;
import javax.jms.JMSException;
import eu.semaine.jms.message.SEMAINEMessage;

/**
 * @author marc
 *
 */
public class BytesSender extends Sender 
{

	/**
	 * @param topicName
	 * @param datatype
	 * @param source
	 * @throws JMSException
	 */
	public BytesSender(String topicName, String datatype, String source)
			throws JMSException {
		super(topicName, datatype, source);
	}

	/**
	 * @param jmsUrl
	 * @param jmsUser
	 * @param jmsPassword
	 * @param topicName
	 * @param datatype
	 * @param source
	 * @throws JMSException
	 */
	public BytesSender(String jmsUrl, String jmsUser, String jmsPassword,
			String topicName, String datatype, String source)
			throws JMSException {
		super(jmsUrl, jmsUser, jmsPassword, topicName, datatype, source);
	}

	/**
	 * Send a bytes message via this sender.
	 * This will send a message to the registered topic 
	 * with the following message properties:
	 * <ul>
	 *   <li><code>datatype</code> is a String property containing the value produced by {@link #getDatatype()};</li>
	 *   <li><code>source</code> is a String property containing the value produced by {@link #getSource()};</li>
	 *   <li><code>usertime</code> is a long property containing the value of parameter <code>usertime</code>;</li>
	 *   <li>if the message is periodic ({@link #isPeriodic()} returns <code>true</code>),
	 *   <code>period</code> is an int property containing the value returned by {@link #getPeriod()};</li>
	 *   <li>else, the message is event-based. <code>event</code> is a String property; as
	 *   this method does not specify an event type, the default value <code>single</code> is assumed.</li>
	 * </ul>
	 * Furthermore, if {@link #getTimeToLive()} returns a non-zero value, the message will
	 * contain a header field <code>JMSExpiration</code> containing the time when the message
	 * will expire.
	 * @param bytes the message data, as a byte array.
	 * @param usertime the "user" time that this message refers to,
	 * in milliseconds since 1970.
	 * @throws IllegalStateException if the connection is not started or the sender is in event-based mode.
	 */
	public void sendBytesMessage(byte[] bytes, long usertime)
	throws JMSException
	{
		if (!isConnectionStarted)
			throw new IllegalStateException("Connection is not started!");
		BytesMessage message = session.createBytesMessage();
		message.writeBytes(bytes);
		fillMessageProperties(message, usertime);
		if (isPeriodic())
			message.setIntProperty(SEMAINEMessage.PERIOD, getPeriod());
		else // event-based
			message.setStringProperty(SEMAINEMessage.EVENT, Event.single.toString());
		producer.send(message);
	}
}