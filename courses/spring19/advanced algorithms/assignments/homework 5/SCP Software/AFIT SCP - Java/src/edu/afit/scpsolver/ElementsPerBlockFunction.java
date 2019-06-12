package edu.afit.scpsolver;

import edu.afit.scpsolver.P;
import java.util.Comparator;

public class ElementsPerBlockFunction implements Comparator<P> {
	public int compare(P p1, P p2) {
		return p1.getGroup().size() - p2.getGroup().size();
	}

	public String toString() {
		return "ElementsPerBlockFunction: Sorted blocks (p) from smallest number of elements per set to greatest number of elements per set.";
	}
}