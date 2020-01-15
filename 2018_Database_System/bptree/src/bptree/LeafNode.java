package bptree;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Scanner;

public class LeafNode extends Node {
	
	protected ArrayList<Pair<Integer,Integer>> keys;
	protected LeafNode rightSiblings;
	
	public LeafNode() {
		this.keys=new ArrayList<Pair<Integer,Integer>>();
		this.rightSiblings=null;
	}
	
	public boolean isOverSize() {
		return this.keys.size() == this.sizeOfKey;
	}
	
	public boolean isUnderSize() {
		return this.keys.size() < (this.sizeOfKey-1)/2;
	}
	
	public int getSmallestKey() {
		return this.keys.get(0).key;
	}
	
	public LeafNode makeTree(Scanner inputIndex,LeafNode leftSiblings) {
		String now;
		while(true) {
			now=inputIndex.next();
			if(now.equals("Up")) break;
			String next=inputIndex.next();
			this.keys.add(new Pair<Integer,Integer>(Integer.parseInt(now),Integer.parseInt(next)));
		}
		if(leftSiblings != null) leftSiblings.rightSiblings=this;
		return this;
	}
	
	public void writeTree(PrintWriter outputIndex) {
		for(Pair<Integer,Integer> now:this.keys)
			outputIndex.print(now.key + " " + now.value + " ");
		outputIndex.println("Up");
	}

	public void writeToIndexFile(PrintWriter outputIndex) {
		outputIndex.println(Boolean.FALSE + " " + sizeOfKey);
		this.writeTree(outputIndex);
	}
	
	public void searchSingleKey(int key) {
		for(Pair<Integer,Integer> now:this.keys) {
			if(key == now.key) {
				System.out.println(now.value);
				return;
			}
		}
		System.out.println("NOT FOUND");
	}
	
	public void searchRangeKey(int startKey,int endKey) {
		LeafNode pos=this;
		boolean endCheck=false,printCheck=false;
		while(!endCheck && pos != null) {
			endCheck=false;
			for(Pair<Integer,Integer> now:pos.keys) {
				if(now.key >= startKey && now.key <= endKey) {
					System.out.println(now.key + "," + now.value);
					printCheck=true;
				}
				else if(now.key > endKey) {
					endCheck=true;
					break;
				}
			}
			if(endCheck) pos=null;
			else pos=pos.rightSiblings;
		}
		if(!printCheck) System.out.println("NOT FOUND");
	}
	
	public Node splitAndUp(Node root,Node up) {
		int i,mid=this.keys.size()/2;
		Pair<Integer,Node> newPair;
		LeafNode newRightNode=new LeafNode();
		newRightNode.sizeOfKey=root.sizeOfKey;
		for(i=mid; i<this.keys.size();) {
			newRightNode.keys.add(new Pair<Integer,Integer>(this.keys.get(mid)));
			this.keys.remove(mid);
		}
		newPair=new Pair<Integer,Node>(newRightNode.keys.get(0).key,this);
		newRightNode.rightSiblings=this.rightSiblings;
		this.rightSiblings=newRightNode;
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
		int i;
		for(i=0; i<this.keys.size(); i++) {
			if(key < this.keys.get(i).key) {
				this.keys.add(i,new Pair<Integer,Integer>(key,value));
				if(this.isOverSize()) root=this.splitAndUp(root,up);
				return root;
			}
			else if(key == this.keys.get(i).key) {
				System.out.println(key + " Insertion fail : It is already in tree");
				return root;
			}
		}
		this.keys.add(new Pair<Integer,Integer>(key,value));
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
					LeafNode leftChild;
					leftChild=(LeafNode)up.keys.get(i-1).value;
					if(leftChild.keys.size()-1 >= (this.sizeOfKey-1)/2) {
						this.keys.add(0,leftChild.keys.get(leftChild.keys.size()-1));
						leftChild.keys.remove(leftChild.keys.size()-1);
						up.keys.get(i-1).key=this.keys.get(0).key;
						return root;
					}
				}
				if(i+1 <= up.keys.size()) {
					if(check == 0) check=2;
					LeafNode rightChild;
					if(i+1 == up.keys.size()) rightChild=(LeafNode)up.rightmostChild;
					else rightChild=(LeafNode)up.keys.get(i+1).value;
					if(rightChild.keys.size()-1 >= (this.sizeOfKey-1)/2) {
						this.keys.add(rightChild.keys.get(0));
						rightChild.keys.remove(0);
						up.keys.get(i).key=rightChild.keys.get(0).key;
						return root;
					}
				}
				if(check == 1) {
					LeafNode leftChild;
					leftChild=(LeafNode)up.keys.get(i-1).value;
					for(j=0; j<this.keys.size();) {
						leftChild.keys.add(this.keys.get(j));
						this.keys.remove(j);
					}
					leftChild.rightSiblings=this.rightSiblings;
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
					LeafNode rightChild;
					if(i+1 == up.keys.size()) rightChild=(LeafNode)up.rightmostChild;
					else rightChild=(LeafNode)up.keys.get(i+1).value;
					for(j=0; j<rightChild.keys.size();) {
						this.keys.add(rightChild.keys.get(j));
						rightChild.keys.remove(j);
					}
					this.rightSiblings=rightChild.rightSiblings;
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
		int i;
		for(i=0; i<this.keys.size(); i++) {
			if(key == this.keys.get(i).key) {
				this.keys.remove(i);
				if(this != root && this.isUnderSize()) root=this.mergeAndUp(root,up);
				return root;
			}
		}
		System.out.println(key + " Deletion fail : It is not in tree");
		return root;
	}
	
	public Node changeKey(Node root,int key) {
		return root;
	}
	
}
