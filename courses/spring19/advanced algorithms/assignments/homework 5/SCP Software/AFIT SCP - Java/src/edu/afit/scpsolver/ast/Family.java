package edu.afit.scpsolver.ast;

import edu.afit.scpsolver.ast.ASTNode;
import edu.afit.scpsolver.ast.ASTVisitor;
import edu.afit.scpsolver.ast.RSet;

public class Family extends ASTNode {
	private RSet r;
	private int id;
	private int cost;
	private int costPerElement;

	public Family(RSet r, Integer i, int id) {
		this.r = r;
		this.cost = i.intValue();
		this.id = id;
		this.costPerElement = (int) Math.ceil((double) this.cost / (double) r.getR().size() * 10000.0D);
	}

	public Family(Family family) {
		this.r = family.getRSet();
		this.cost = family.getCost();
		this.id = family.getId();
		this.costPerElement = family.getCostPerElement();
	}

	public int getId() {
		return this.id;
	}

	public int getCost() {
		return this.cost;
	}

	public void accept(ASTVisitor v) {
		if (v.visit(this)) {
			this.getRSet().accept(v);
		}

	}

	public RSet getRSet() {
		return this.r;
	}

	public String toString() {
		return Integer.toString(this.id) + ": (" + this.r.toString() + ", " + Integer.toString(this.cost) + ")";
	}

	public int hashCode() {
		return this.id;
	}

	public int getCostPerElement() {
		return this.costPerElement;
	}

	public void setCostPerElement(int costPerElement) {
		this.costPerElement = costPerElement;
	}
}