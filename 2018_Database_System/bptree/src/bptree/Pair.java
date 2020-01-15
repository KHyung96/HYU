package bptree;

public class Pair<Interger,V> {
	
	public int key;
	public V value;
	
	public Pair(int key,V value) {
		this.key=key;
		this.value=value;
	}
	
	public Pair(Pair<Integer,V> newPair) {
		this.key=newPair.key;
		this.value=newPair.value;
	}
	
}