package edu.afit.scpsolver;

import edu.afit.scpsolver.ast.Family;
import java.util.Comparator;

public class IDFunction implements Comparator<Family> {
	public int compare(Family f1, Family f2) {
		return f1.getId() - f2.getId();
	}

	public String toString() {
		return "IDFunction: Sorted families (j) in the order they were read from the file.";
	}
}