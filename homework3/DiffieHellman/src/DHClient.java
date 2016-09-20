import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.Socket;

/**
 *	Author: Gregory Smith
 *	Project: Homework 3
 *	File: DHClient.java
 *	Last Modified: 09/18/2016 
 */

public class DHClient {
	private static final int PORT = 11000;

	public static void main(String[] args) {
		try{
			String line;
			String response;
			
			BufferedReader fromUser = new BufferedReader(new InputStreamReader(System.in));
			Socket clientSocket = new Socket("localhost", PORT);
			
			DataOutputStream toServer = new DataOutputStream(clientSocket.getOutputStream());
			BufferedReader fromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
			
			System.out.println("Please enter a line.");
			
			line = fromUser.readLine();
			toServer.writeBytes(line + '\n');
			
			response = fromServer.readLine();
			System.out.println("From Server: " + response);
			
			clientSocket.close();
		}
		catch(Exception e){
			System.err.println(e.getMessage());
		}
		finally{
			
		}
	}
}
