package bptree;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.StringTokenizer;

public class BpTree {
	
	private static void indexFileCreation(String indexFileName,int sizeOfKey) {
		PrintWriter outputIndex=null;
		try {
			outputIndex=new PrintWriter(new BufferedOutputStream(new FileOutputStream(indexFileName)));
			if(sizeOfKey < 3) System.out.println("Creation Fail : Size of key is too small");
			else {
				Node root=new LeafNode();
				root.sizeOfKey=sizeOfKey;
				root.writeToIndexFile(outputIndex);
			}
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		finally {
			try {
				if(outputIndex != null) outputIndex.close();
			}
			catch(Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	private static void keyInsertion(String indexFileName,String dataFileName) {
		Scanner inputIndex=null;
		PrintWriter outputIndex=null;
		Scanner inputData=null;
		try {
			inputIndex=new Scanner(new BufferedInputStream(new FileInputStream(indexFileName)));
			inputData=new Scanner(new BufferedInputStream(new FileInputStream(dataFileName)));
			Node root=null;
			if(Boolean.parseBoolean(inputIndex.next())) root=new Node();
			else root=new LeafNode();
			root.sizeOfKey=Integer.parseInt(inputIndex.next());
			root.makeTree(inputIndex,null);
			String line;
			StringTokenizer now;
			while(inputData.hasNextLine()) {
				line=inputData.nextLine();
				now=new StringTokenizer(line,",");
				root=root.insertKey(root,root,Integer.parseInt(now.nextToken()),Integer.parseInt(now.nextToken()));
			}
			outputIndex=new PrintWriter(new BufferedOutputStream(new FileOutputStream(indexFileName)));
			root.writeToIndexFile(outputIndex);
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		finally {
			try {
				if(inputIndex != null) inputIndex.close();
				if(outputIndex != null) outputIndex.close();
				if(inputData != null) inputData.close();
			}
			catch(Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	private static void keyDeletion(String indexFileName,String dataFileName) {
		Scanner inputIndex=null;
		PrintWriter outputIndex=null;
		Scanner inputData=null;
		try {
			inputIndex=new Scanner(new BufferedInputStream(new FileInputStream(indexFileName)));
			inputData=new Scanner(new BufferedInputStream(new FileInputStream(dataFileName)));
			Node root=null;
			if(Boolean.parseBoolean(inputIndex.next())) root=new Node();
			else root=new LeafNode();
			root.sizeOfKey=Integer.parseInt(inputIndex.next());
			root.makeTree(inputIndex,null);
			String line;
			int deleteKey;
			while(inputData.hasNextLine()) {
				line=inputData.nextLine();
				deleteKey=Integer.parseInt(line);
				root=root.deleteKey(root,root,deleteKey);
				root=root.changeKey(root,deleteKey);
			}
			outputIndex=new PrintWriter(new BufferedOutputStream(new FileOutputStream(indexFileName)));
			root.writeToIndexFile(outputIndex);
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		finally {
			try {
				if(inputIndex != null) inputIndex.close();
				if(outputIndex != null) outputIndex.close();
				if(inputData != null) inputData.close();
			}
			catch(Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	private static void singleKeySearch(String indexFileName,int key) {
		Scanner inputIndex=null;
		try {
			inputIndex=new Scanner(new BufferedInputStream(new FileInputStream(indexFileName)));
			Node root=null;
			if(Boolean.parseBoolean(inputIndex.next())) root=new Node();
			else root=new LeafNode();
			root.sizeOfKey=Integer.parseInt(inputIndex.next());
			root.makeTree(inputIndex,null);
			root.searchSingleKey(key);
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		finally {
			try {
				if(inputIndex != null) inputIndex.close();
			}
			catch(Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	private static void rangeKeySearch(String indexFileName,int startKey,int endKey) {
		Scanner inputIndex=null;
		try {
			inputIndex=new Scanner(new BufferedInputStream(new FileInputStream(indexFileName)));
			Node root=null;
			if(Boolean.parseBoolean(inputIndex.next())) root=new Node();
			else root=new LeafNode();
			root.sizeOfKey=Integer.parseInt(inputIndex.next());
			root.makeTree(inputIndex,null);
			root.searchRangeKey(startKey,endKey);
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		finally {
			try {
				if(inputIndex != null) inputIndex.close();
			}
			catch(Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	public static void main(String[] args) {
		String path=System.getProperty("user.dir") + "/";
		switch(args[0]) {
			case "-c" :
				BpTree.indexFileCreation(path+args[1],Integer.parseInt(args[2]));
				break;
			case "-i" :
				BpTree.keyInsertion(path+args[1],path+args[2]);
				break;
			case "-d" :
				BpTree.keyDeletion(path+args[1],path+args[2]);
				break;
			case "-s" :
				BpTree.singleKeySearch(path+args[1],Integer.parseInt(args[2]));
				break;
			case "-r" :
				BpTree.rangeKeySearch(path+args[1],Integer.parseInt(args[2]),Integer.parseInt(args[3]));
				break;
			default :
				System.out.println("Command Input Error");
		}
	}
	
}
