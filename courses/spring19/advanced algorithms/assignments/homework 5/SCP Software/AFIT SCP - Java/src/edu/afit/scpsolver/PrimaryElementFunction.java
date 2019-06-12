package edu.afit.scpsolver;

import edu.afit.scpsolver.P;
import java.util.Comparator;

public class PrimaryElementFunction implements Comparator<P> {
	public int compare(P p1, P p2) {
		return p1.getPrimaryElement().intValue() - p2.getPrimaryElement().intValue();
	}

	public String toString() {
		return "Sorted blocks (p) from smallest elements to largest elements.";
	}
}