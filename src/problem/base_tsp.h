/*****************************************************************************
 *   Copyright (C) 2004-2014 The PaGMO development team,                     *
 *   Advanced Concepts Team (ACT), European Space Agency (ESA)               *
 *   http://apps.sourceforge.net/mediawiki/pagmo                             *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Developers  *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Credits     *
 *   act@esa.int                                                             *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 *****************************************************************************/

#ifndef PAGMO_PROBLEM_BASE_TSP_H
#define PAGMO_PROBLEM_BASE_TSP_H

#include "base.h"
#include <vector>

namespace pagmo{ namespace problem {

/// Boost graph type.
/**
 * std::vector (vecS) are fastest for iterators: http://www.boost.org/doc/libs/1_55_0/libs/graph/doc/using_adjacency_list.html
 * External properties can also be added: http://www.boost.org/doc/libs/1_55_0/libs/graph/doc/quick_tour.html
 */
//typedef boost::property<boost::vertex_index_t, int, boost::property<boost::vertex_name_t, std::string> > tsp_vertex_property;
// no_property automatically adds vertex_index_t, int for vecS
typedef boost::property<boost::edge_weight_t, double> tsp_edge_properties;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, tsp_edge_properties> tsp_graph;

/// Base TSP.
/**
 *
 * All integer optimization problems must extend this class in order to be solved by Ant Colony Optimization.
 *
 * m_eta is the heuristic information matrix. It represent an a priori knowledge on the problem and has to be set in problem implementation.
 * m_eta[k][i][j] represents the cost of having the j-th value in position k of the chromosome
 * and the i-th value in position k+1. This type of info can later used by the algorithm to guide the optimization.
 * For example in the algorithm::aco it makes the ant prefer clever steps. In fact the probability for a particular step to be chosen
 * is proportional to the the product between the m_eta value of that step (heuristic information) and the amount of pheromone left
 * by previous ants on that step.
 *
 * @author Florin Schimbinschi (florinsch@gmail.com)
 */

class __PAGMO_VISIBLE base_tsp : public base
{
	public:
		base_tsp(size_type, size_type = 0, size_type = 0);
		/**
		 * Checks if a partial solution x is feasible. x.size() may be less than problem length.
		 * @returns true if there is at least one solution having x as a prefix that is feasible. False otherwise
		 */
		virtual bool check_partial_feasibility(const decision_vector &x) const;

		/**
		 * Gets the heuristic information matrix
		 * @returns const reference to m_eta: the heuristic information matrix
		 */
		//const tsp_graph &get_graph() const;
                const std::vector<std::vector<std::vector<fitness_vector> > > &get_heuristic_information_matrix() const;

	protected:
		/**
		 * Set the heuristic information matrix. This should be overridden by subclasses to set the proper heuristic information
		 * matrix for the problem
		 */
		virtual void set_heuristic_information_matrix();

		/**
		 * The boost graph
		 */
                tsp_graph m_graph;
                std::vector<std::vector<std::vector<fitness_vector> > > m_eta;
		/**
		 * Allocate memory for the heuristic information matrix. That must be
		 * called at the begining of each set_heuristic_information_matrix() implementation
		 */
		void create_heuristic_information_matrix();
};

}} //namespaces

#endif // PAGMO_PROBLEM_BASE_TSP_H