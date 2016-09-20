import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

/**
 *	Author: Gregory Smith
 *	Project: Homework 3
 *	File: DHClient.java
 *	Last Modified: 09/18/2016 
 */

public class DHClient {
	private static final int PORT = 11001;
	
	private static final int G = 1907;
	private static final int P = 784313;
	private static final int S_A = 160011;

	public static void main(String[] args) throws IOException {
	    Socket clientSocket = null;
	    BufferedWriter toFile = null;
		try{
			// To output to file
			toFile = new BufferedWriter(new FileWriter("output.txt", true));
			
			// Connect to server
			clientSocket = new Socket("localhost", PORT);
			DataOutputStream toServer = new DataOutputStream(clientSocket.getOutputStream());
			BufferedReader fromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
			
			// Calculate A = g^S_A mod p
			long A = EXPO.exponentiate(G, S_A, P);
			
			// Output A
			System.out.println("Sending Alice's number: " + A);
			toFile.write("Sending Alice's number: " + A + "\r\n");
			// Send A
			toServer.writeBytes(A + "\n");
			
			// Receive B
			long B = Long.parseLong(fromServer.readLine());
			// Output B
			System.out.println("Alice received Bob's number: " + B);
			toFile.write("Alice received Bob's number: " + B + "\r\n");
			
			// Calculate Key = B^S_A mod p
			long key = EXPO.exponentiate((int)B, S_A, P);
			// Output key
			System.out.println("Calculated Key: " + key);
			toFile.write("Alice calculated Key: " + key + "\r\n");
			// Send Key
			toServer.writeBytes(key + "\n");
			
			// Receive Bob's Key
			long keyBob = Long.parseLong(fromServer.readLine());
			System.out.println("Received Bob's key: " + keyBob);
			toFile.write("Alice received Bob's key: " + keyBob + "\r\n");
			// Check that Keys match
			if(keyBob == key){
			    System.out.println("Keys match!");
			}
			toFile.flush();
			// Output key
			toFile.write("\r\n\r\nSUMMARY\r\nAlice's number = " + A + "\r\nBob's number = " + B + "\r\nShared Key = " + key + "\r\n");
		}
		catch(Exception e){
			System.err.println(e.getMessage());
		}
		finally{
		    if(clientSocket != null)
		        clientSocket.close();
		    if(toFile != null)
		        toFile.close();
		}
	}
}
