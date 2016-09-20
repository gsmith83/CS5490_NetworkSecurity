
/**
 *	Author: Gregory Smith
 *	Project: Homework 3
 *	File: DHServer.java
 *	Last Modified: 09/18/2016 
 */
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

public class DHServer {
    private static final int PORT = 11001;

    private static final int G = 1907;
    private static final int P = 784313;
    private static final int S_B = 12067;

    public static void main(String[] args){
		String clientLine = null;
		while(true){
			try{
			    // to output to file
			    BufferedWriter toFile = new BufferedWriter(new FileWriter("output.txt", true)); // only client will write to output file
			    
			    System.out.println("Waiting for client.");
			    
			    // Set up server socket and connection to client
				ServerSocket srvSocket = new ServerSocket(PORT);
				Socket connSocket = srvSocket.accept();
				
				System.out.println("Client connected");
				
				BufferedReader fromClient = new BufferedReader(new InputStreamReader(connSocket.getInputStream()));
				DataOutputStream toClient = new DataOutputStream(connSocket.getOutputStream());
				
				// wait to get the line from DHClient (Alice's number)
				clientLine = fromClient.readLine();
				long A = Long.parseLong(clientLine);
				System.out.println("Bob received Alice's number: " + clientLine);
				toFile.write("Bob received Alice's number: " + clientLine + "\r\n");

				// calculate B = g^s_b mod p
				long B = EXPO.exponentiate(G, S_B, P);
				// output B
				System.out.println("Bob's number is " + B);
				toFile.write("Sending Bob's number: " + B + "\r\n");
				// send B
				toClient.writeBytes(B + "\n");
				
				// wait for Alice's key
				long keyAlice = Long.parseLong(fromClient.readLine());
				System.out.println("Received Alice's key: " + keyAlice);
				toFile.write("Received Alice's key: " + keyAlice + "\r\n");
				
				// calculate key
				long key = EXPO.exponentiate((int)A, S_B, P);
				
				// check that keys match
				System.out.println("Calculated Bob's key: " + key);
				toFile.write("Bob calculated key: " + key + "\r\n");
				
				if(key == keyAlice)
				    System.out.println("Keys match!");
				
				toFile.flush();
				//send key
				toClient.writeBytes(key + "\n");
				
				srvSocket.close();
				connSocket.close();
				fromClient.close();
				toClient.close();
				toFile.close();
			}
			catch(Exception e){
				System.err.println(e.getMessage());
			}
		}
	}
}
