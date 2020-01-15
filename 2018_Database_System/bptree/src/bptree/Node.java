package bptree;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Scanner;

public class Node {

	protected int sizeOfKey;
	protected ArrayList<Pair<Integer,Node>> keys;
	protected Node rightmostChild;
	
	public Node() {
		this.keys=new ArrayList<Pair<Integer,Node>>();
		this.rightmostChild=null;
	}

	public boolean isOverSize() {
		return this.keys.size() == this.sizeOfKey;
	}
	
	public boolean isUnderSize() {
		return this.keys.size() < (this.sizeOfKey-1)/2;
	}
	
	public int getSmallestKey() {
		return this.keys.get(0).value.getSmallestKey();
	}
	
	public LeafNode makeTree(Scanner inputIndex,LeafNode leftSiblings) {
		String now;
		while(true) {
			now=inputIndex.next();
			if(now.equals("Leaf")) {
				LeafNode newNode=new LeafNode();
				newNode.sizeOfKey=this.sizeOfKey;
				String next=inputIndex.next();
				if(next.equals("Right")) {
					this.rightmostChild=newNode;
					leftSiblings=newNode.makeTree(inputIndex,leftSiblings);
					break;
				}
				else {
					this.keys.add(new Pair<Integer,Node>(Integer.parseInt(next),newNode));
					leftSiblings=newNode.makeTree(inputIndex,leftSiblings);
				}
			}
			else {
				Node newNode=new Node();
				newNode.sizeOfKey=this.sizeOfKey;
				if(now.equals("Right")) {
					this.rightmostChild=newNode;
					leftSiblings=newNode.makeTree(inputIndex,leftSiblings);
					break;
				}
				else {
					this.keys.add(new Pair<Integer,Node>(Integer.parseInt(now),newNode));
					leftSiblings=newNode.makeTree(inputIndex,leftSiblings);
				}
			}
		}
		return leftSiblings;
	}
	
	public void writeTree(PrintWriter outputIndex) {
		for(Pair<Integer,Node> now:this.keys) {
			if(now.value instanceof LeafNode) outputIndex.print("Leaf ");
			outputIndex.print(now.key + " ");
			now.value.writeTree(outputIndex);
		}
		if(this.rightmostChild instanceof LeafNode) outputIndex.print("Leaf ");
		outputIndex.print("Right ");
		this.rightmostChild.writeTree(outputIndex);
	}

	public void writeToIndexFile(PrintWriter outputIndex) {
		outputIndex.println(Boolean.TRUE + " " + sizeOfKey);
		this.writeTree(outputIndex);
	}

	public void searchSingleKey(int key) {
		for(int i=0; i<this.keys.size()-1; i++)
			System.out.print(this.keys.get(i).key + ",");
		System.out.println(this.keys.get(this.keys.size()-1).key);
		for(Pair<Integer,Node> now:this.keys) {
			if(key < now.key) {
				now.value.searchSingleKey(key);
				return;
			}
		}
		this.rightmostChild.searchSingleKey(key);
	}
	
	public void searchRangeKey(int startKey,int endKey) {
		for(Pair<Integer,Node> now:this.keys) {
			if(startKey < now.key) {
				now.value.searchRangeKey(startKey,endKey);
				return;
			}
		}
		this.rightmostChild.searchRangeKey(startKey,endKey);
	}
	
	public Node splitAndUp(Node root,Node up) {
		int i,mid=this.keys.size()/2;
		Pair<Integer,Node> newPair;
		Node newRightNode=new Node();
		newRightNode.sizeOfKey=root.sizeOfKey;
		for(i=mid+1; i<this.keys.size();) {
			newRightNode.keys.add(new Pair<Integer,Node>(this.keys.get(mid+1)));
			this.keys.remove(mid+1);
		}
		newRightNode.rightmostChild=this.rightmostChild;
		this.rightmostChild=this.keys.get(mid).value;
		newPair=new Pair<Integer,Node>(this.keys.get(mid).key,this);
		this.keys.remove(mid);
		if(this == root) {
			root=new Node();
			root.sizeOfKey=this.sizeOfKey;
			root.keys.add(newPair);
			root.rightmostChild=newRightNode;
		}
		else {
			for(i=0; i<up.keys.size(); i++) {
				if(newPair.key < up.keys.get(i).key) {
					up.keys.add(i,newPair);
					up.keys.get(i+1).value=newRightNode;
					return root;
				}
			}
			up.keys.add(newPair);
			up.rightmostChild=newRightNode;
		}
		return root;
	}
	
	public Node insertKey(Node root,Node up,int key,int value) {
		for(Pair<Integer,Node> now:this.keys) {
			if(key < now.key) {
				now.value.insertKey(root,this,key,value);
				if(this.isOverSize()) root=this.splitAndUp(root,up);
				return root;
			}
		}
		this.rightmostChild.insertKey(root,this,key,value);
		if(this.isOverSize()) root=this.splitAndUp(root,up);
		return root;
	}
	
	public Node mergeAndUp(Node root,Node up) {
		int i,j,check=0;
		Node now=null;
		for(i=0; i<=up.keys.size(); i++) {
			if(i == up.keys.size()) now=up.rightmostChild;
			else now=up.keys.get(i).value;
			if(now == this) {
				if(i-1 >= 0) {
					check=1;
					Node leftChild;
					leftChild=up.keys.get(i-1).value;
					if(leftChild.keys.size()-1 >= (this.sizeOfKey-1)/2) {
						if(this.keys.size() == 0) this.keys.add(0,new Pair<Integer,Node>(this.rightmostChild.getSmallestKey(),leftChild.rightmostChild));
						else this.keys.add(0,new Pair<Integer,Node>(this.keys.get(0).value.getSmallestKey(),leftChild.rightmostChild));
						Node temp=leftChild.keys.get(leftChild.keys.size()-1).value;
						leftChild.keys.remove(leftChild.keys.size()-1);
						leftChild.rightmostChild=temp;
						up.keys.get(i-1).key=this.getSmallestKey();
						return root;
					}
				}
				if(i+1 <= up.keys.size()) {
					if(check == 0) check=2;
					Node rightChild;
					if(i+1 == up.keys.size()) rightChild=up.rightmostChild;
					else rightChild=up.keys.get(i+1).value;
					if(rightChild.keys.size()-1 >= (this.sizeOfKey-1)/2) {
						this.keys.add(new Pair<Integer,Node>(rightChild.keys.get(0).value.getSmallestKey(),this.rightmostChild));
						this.rightmostChild=rightChild.keys.get(0).value;
						rightChild.keys.remove(0);
						up.keys.get(i).key=rightChild.getSmallestKey();
						return root;
					}
				}
				if(check == 1) {
					Node leftChild;
					leftChild=up.keys.get(i-1).value;
					leftChild.keys.add(new Pair<Integer,Node>(up.keys.get(i-1).key,leftChild.rightmostChild));
					for(j=0; j<this.keys.size();) {
						leftChild.keys.add(this.keys.get(j));
						this.keys.remove(j);
					}
					leftChild.rightmostChild=this.rightmostChild;
					if(up.keys.size() == 1) {
						up.rightmostChild=up.keys.get(0).value;
						up.keys.remove(0);
					}
					else {
						if(up.rightmostChild == this) {
							up.keys.remove(up.keys.size()-1);
							up.rightmostChild=leftChild;
						}
						else {
							for(j=i-1; j<up.keys.size()-1; j++)
								up.keys.set(j,up.keys.get(j+1));
							up.keys.remove(up.keys.size()-1);
							if(i == up.keys.size()) up.keys.get(i-1).key=up.rightmostChild.getSmallestKey();
							else up.keys.get(i-1).key=up.keys.get(i).value.getSmallestKey();
							up.keys.get(i-1).value=leftChild;
						}
					}
				}
				else if(check == 2) {
					Node rightChild;
					if(i+1 == up.keys.size()) rightChild=up.rightmostChild;
					else rightChild=up.keys.get(i+1).value;
					this.keys.add(new Pair<Integer,Node>(up.keys.get(i).key,this.rightmostChild));
					for(j=0; j<rightChild.keys.size();) {
						this.keys.add(rightChild.keys.get(j));
						rightChild.keys.remove(j);
					}
					this.rightmostChild=rightChild.rightmostChild;
					if(up.keys.size() == 1) {
						up.rightmostChild=up.keys.get(0).value;
						up.keys.remove(0);
					}
					else {
						if(up.rightmostChild == rightChild) {
							up.keys.remove(up.keys.size()-1);
							up.rightmostChild=this;
						}
						else {
							for(j=i; j<up.keys.size()-1; j++)
								up.keys.set(j,up.keys.get(j+1));
							up.keys.remove(up.keys.size()-1);
							if(i+1 == up.keys.size()) up.keys.get(i).key=up.rightmostChild.getSmallestKey();
							else up.keys.get(i).key=up.keys.get(i+1).value.getSmallestKey();
							up.keys.get(i).value=this;
						}
					}
				}
				break;
			}
		}
		return root;
	}
	
	public Node deleteKey(Node root,Node up,int key) {
		for(Pair<Integer,Node> now:this.keys) {
			if(key < now.key) {
				now.value.deleteKey(root,this,key);
				if(this != root && this.isUnderSize()) root=this.mergeAndUp(root,up);
				if(this == root && this.keys.size() == 0) root=this.rightmostChild;
				return root;
			}
		}
		this.rightmostChild.deleteKey(root,this,key);
		if(this != root && this.isUnderSize()) root=this.mergeAndUp(root,up);
		if(this == root && this.keys.size() == 0) root=this.rightmostChild;
		return root;
	}
	
	public Node changeKey(Node root,int key) {
		Pair<Integer,Node> temp=null;
		for(Pair<Integer,Node> now:this.keys) {
			if(key < now.key) {
				if(temp == null) now.value.changeKey(root,key);
				else temp.key=now.value.getSmallestKey();
				return root;
			}
			else if(key == now.key) temp=now;
		}
		if(temp == null) this.rightmostChild.changeKey(root,key);
		else temp.key=this.rightmostChild.getSmallestKey();
		return root;
	}
	
}