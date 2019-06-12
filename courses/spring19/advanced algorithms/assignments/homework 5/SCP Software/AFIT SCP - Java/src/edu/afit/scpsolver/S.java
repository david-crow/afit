package edu.afit.scpsolver;

import edu.afit.scpsolver.P;
import edu.afit.scpsolver.PrimaryElementFunction;
import edu.afit.scpsolver.ast.Family;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;

public class S {
	private ArrayList<P> blocks;
	private int marker;
	private Comparator<P> c;
	private ArrayList<Integer> r;

	public S(ArrayList<P> b, Comparator<P> c) {
		this.marker = -1;
		this.r = new ArrayList();
		this.blocks = new ArrayList(b);
		this.c = c;
	}

	public S(int i) {
		this(new ArrayList(i), new PrimaryElementFunction());
	}

	public S(int i, Comparator<P> c) {
		this(new ArrayList(i), c);
	}

	public boolean isEmpty() {
		return this.blocks.isEmpty();
	}

	public void addBlock(P p) {
		this.blocks.add(p);
		this.sort(this.c);
	}

	public void removeBlock(P p) {
		this.blocks.remove(p);
		this.sort(this.c);
	}

	public P getAt(int i) {
		return (P) this.blocks.get(i);
	}

	public ListIterator<P> listIterator() {
		return this.blocks.listIterator();
	}

	public void resetMarker() {
		this.marker = -1;
	}

	public void setMarker(P p) {
		this.marker = this.blocks.indexOf(p);
	}

	public boolean hasNext() {
		return this.marker + 1 < this.blocks.size();
	}

	public P getFirst() {
		return (P) this.blocks.get(0);
	}

	public P getNext() {
		return (P) this.blocks.get(++this.marker);
	}

	public int getMarker() {
		return this.marker;
	}

	public void sort(Comparator<P> c) {
		this.c = c;
		Collections.sort(this.blocks, c);
		ArrayList sortedR = new ArrayList();
		Iterator arg3 = this.blocks.iterator();

		while (arg3.hasNext()) {
			P p = (P) arg3.next();
			sortedR.add(p.getPrimaryElement());
		}

		this.r = sortedR;
	}

	public ArrayList<Integer> getR() {
		return this.r;
	}

	public String printSHeader() {
		StringBuilder sb = new StringBuilder();
		boolean pad = false;
		int pCount = 0;
		sb.append("S_p |");
		Iterator arg4 = this.blocks.iterator();

		while (arg4.hasNext()) {
			P p = (P) arg4.next();
			int arg5 = 2 * (p.size() - 1);
			sb.append(this.padHeader(arg5));
			Object[] arg10001 = new Object[1];
			++pCount;
			arg10001[0] = Integer.valueOf(pCount);
			sb.append(String.format("%3d", arg10001));
			sb.append(this.padHeader(arg5));
			sb.append("|");
		}

		return sb.toString();
	}

	public String printPHeader() {
		StringBuilder sb = new StringBuilder();
		sb.append("  j |");
		Iterator arg2 = this.blocks.iterator();

		while (arg2.hasNext()) {
			P p = (P) arg2.next();
			sb.append(p.printHeader());
			sb.append("|");
		}

		return sb.toString();
	}

	public String printPRows(List<Integer> RSet) {
		StringBuilder sb = new StringBuilder();
		int column = 0;
		Iterator iter = RSet.iterator();

		while (iter.hasNext()) {
			Integer i = (Integer) ((Integer) iter.next());
			boolean pad = false;
			String r = String.format("%3d", new Object[]{i});
			sb.append(r + " |");

			for (Iterator arg8 = this.blocks.iterator(); arg8.hasNext(); sb.append("|")) {
				P p = (P) arg8.next();
				if (pad) {
					sb.append(p.printPad());
				} else {
					sb.append(p.printRow(i));
				}

				if (p.getPrimaryElement().equals(i)) {
					pad = true;
				}
			}

			++column;
			if (iter.hasNext()) {
				sb.append("\n");
			}
		}

		return sb.toString();
	}

	public String printCost() {
		StringBuilder sb = new StringBuilder();
		sb.append("Cost|");
		Iterator arg2 = this.blocks.iterator();

		while (arg2.hasNext()) {
			P p = (P) arg2.next();
			sb.append(p.printCost());
			sb.append("|");
		}

		return sb.toString();
	}

	public String printLine() {
		StringBuilder sb = new StringBuilder();
		sb.append("    |");
		Iterator arg2 = this.blocks.iterator();

		while (arg2.hasNext()) {
			P p = (P) arg2.next();
			sb.append(p.printLine());
			sb.append("|");
		}

		return sb.toString();
	}

	private String padHeader(int i) {
		StringBuilder sb = new StringBuilder();

		for (int j = 0; j < i; ++j) {
			sb.append(" ");
		}

		return sb.toString();
	}

	public List<P> getBlocks() {
		return this.blocks;
	}

	public Comparator<P> getSComparator() {
		return this.c;
	}

	public Comparator<Family> getPComparator() {
		return ((P) this.blocks.get(0)).getPComparator();
	}
}