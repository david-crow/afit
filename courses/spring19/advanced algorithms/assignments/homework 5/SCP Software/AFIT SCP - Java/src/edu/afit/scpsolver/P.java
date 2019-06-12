package edu.afit.scpsolver;

import edu.afit.scpsolver.CostFunction;
import edu.afit.scpsolver.PrintCommand;
//import edu.afit.scpsolver.P.1;
//import edu.afit.scpsolver.P.2;
//import edu.afit.scpsolver.P.3;
//import edu.afit.scpsolver.P.4;
//import edu.afit.scpsolver.P.5;
import edu.afit.scpsolver.ast.Family;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.ListIterator;

public class P {
	private ArrayList<Family> group;
	private int marker;
	private Integer primaryElement;
	Comparator<Family> c;

	public P(ArrayList<Family> g, Integer i, Comparator<Family> c) {
		this.marker = -1;
		this.group = new ArrayList(g);
		this.primaryElement = i;
		this.c = c;
	}

	public P(Integer i) {
		this(new ArrayList(), i, new CostFunction());
	}

	public P(Integer i, Comparator<Family> c) {
		this(new ArrayList(), i, c);
	}

	public void addFamily(Family f) {
		this.group.add(f);
		this.sort(this.c);
	}

	public void removeFamily(Family f) {
		this.group.remove(f);
		this.sort(this.c);
	}

	public Family getAt(int i) {
		return (Family) this.group.get(i);
	}

	public void resetMarker() {
		this.marker = -1;
	}

	public Family getNext() {
		return (Family) this.group.get(++this.marker);
	}

	public boolean hasNext() {
		return this.marker + 1 < this.group.size();
	}

	public int getMarker() {
		return this.marker;
	}

	public Integer getPrimaryElement() {
		return this.primaryElement;
	}

	public void sort(Comparator<Family> c) {
		this.c = c;
		Collections.sort(this.group, c);
	}

	public int size() {
		return this.group.size();
	}

	// public String printHeader() {
	// return this.print(new 1(this));
	// }

	public String printHeader() {
		return this.print(new PrintCommand() {
			public String print(Family f) {
				return String.format("%3d", f.getId());
			};
		});
	}

	// public String printRow(Integer i) {
	// return this.print(new 2(this, i));
	// }

	public String printRow(Integer i) {
		return this.print(new PrintCommand() {
			public String print(Family f) {
				return f.getRSet().getR().contains(i) ? " 1 " : " 0 ";
			}
		});
	}

	// public String printPad() {
	// return this.print(new 3(this));
	// }

	public String printPad() {
		return this.print(new PrintCommand() {
			public String print(Family f) {
				return "   ";
			}
		});
	}

	// public Object printCost() {
	// return this.print(new 4(this));
	// }

	public Object printCost() {
		return this.print(new PrintCommand() {
			public String print(Family f) {
				return String.format("%3d", new Object[] { Integer.valueOf(f.getCost()) });
			}
		});
	}

	// public String printLine() {
	// return this.print(new 5(this));
	// }

	public String printLine() {
		return this.print(new PrintCommand() {
			public String print(Family f) {
				return "---";
			}
		});
	}

	private String print(PrintCommand p) {
		StringBuilder sb = new StringBuilder();
		Iterator iter = this.group.iterator();

		while (iter.hasNext()) {
			Family f = (Family) ((Family) iter.next());
			sb.append(p.print(f));
			if (iter.hasNext()) {
				if (p.print(f).equals("---")) {
					sb.append("-");
				} else {
					sb.append("|");
				}
			}
		}

		return sb.toString();
	}

	public ListIterator<Family> listIterator() {
		return this.group.listIterator();
	}

	public ArrayList<Family> getGroup() {
		return this.group;
	}

	public Comparator<Family> getPComparator() {
		return this.c;
	}

	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("{[" + this.primaryElement.toString() + "], (" + this.marker + "), {");
		Iterator iter = this.group.iterator();

		while (iter.hasNext()) {
			Family f = (Family) ((Family) iter.next());
			sb.append(f.getId());
			if (iter.hasNext()) {
				sb.append(" ");
			}
		}

		sb.append("}}");
		return sb.toString();
	}
}