/**
 *	Author: Gregory Smith
 *	Project: Homework 3
 *	File: DHServer.java
 *	Last Modified: 09/18/2016 
 */
import java.io.*;
import java.net.*;

public class DHServer {
	private static final int PORT = 11000;
	
	public static void main(String[] args){
		String clientLine = null;
		while(true){
			try{
				ServerSocket srvSocket = new ServerSocket(PORT);
				Socket connSocket = srvSocket.accept();
				
				BufferedReader fromClient = new BufferedReader(new InputStreamReader(connSocket.getInputStream()));
				DataOutputStream toClient = new DataOutputStream(connSocket.getOutputStream());
				
				clientLine = fromClient.readLine();
				
				System.out.println("Received: " + clientLine);
				
				// do work
				
				// reply to client
				toClient.writeBytes("Hey there!\n");
				
				srvSocket.close();
				connSocket.close();
				fromClient.close();
				toClient.close();
			}
			catch(Exception e){
				System.err.println(e.getMessage());
			}
		}
	}
}
