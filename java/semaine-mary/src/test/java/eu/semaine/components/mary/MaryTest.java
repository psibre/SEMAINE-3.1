package eu.semaine.components.mary;

import static org.junit.Assert.*;

import javax.sound.sampled.AudioInputStream;

import marytts.LocalMaryInterface;
import marytts.MaryInterface;
import marytts.voice.CmuSltHsmm.Config;

import org.junit.Test;

public class MaryTest {

	@Test
	public void canProcessTextToSpeech() throws Exception {
		MaryInterface mary = new LocalMaryInterface();
		mary.setVoice(new Config().getName());
		AudioInputStream audio = mary.generateAudio("Hello world");
		assertNotNull(audio);
	}

}
