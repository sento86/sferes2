//| This file is a part of the sferes2 framework.
//| Copyright 2009, ISIR / Universite Pierre et Marie Curie (UPMC)
//| Main contributor(s): Jean-Baptiste Mouret, mouret@isir.fr
//|
//| This software is a computer program whose purpose is to facilitate
//| experiments in evolutionary computation and evolutionary robotics.
//| 
//| This software is governed by the CeCILL license under French law
//| and abiding by the rules of distribution of free software.  You
//| can use, modify and/ or redistribute the software under the terms
//| of the CeCILL license as circulated by CEA, CNRS and INRIA at the
//| following URL "http://www.cecill.info".
//| 
//| As a counterpart to the access to the source code and rights to
//| copy, modify and redistribute granted by the license, users are
//| provided only with a limited warranty and the software's author,
//| the holder of the economic rights, and the successive licensors
//| have only limited liability.
//|
//| In this respect, the user's attention is drawn to the risks
//| associated with loading, using, modifying and/or developing or
//| reproducing the software by the user in light of its specific
//| status of free software, that may mean that it is complicated to
//| manipulate, and that also therefore means that it is reserved for
//| developers and experienced professionals having in-depth computer
//| knowledge. Users are therefore encouraged to load and test the
//| software's suitability as regards their requirements in conditions
//| enabling the security of their systems and/or data to be ensured
//| and, more generally, to use and operate it in the same conditions
//| as regards security.
//|
//| The fact that you are presently reading this means that you have
//| had knowledge of the CeCILL license and that you accept its terms.




#define BOOST_TEST_DYN_LINK 
#define BOOST_TEST_MODULE rank_simple2


#include <boost/test/unit_test.hpp>
#include <cmath>
#include <iostream>
#include <sferes/phen/parameters.hpp>
#include <sferes/gen/evo_float.hpp>
#include <sferes/ea/rank_simple.hpp>
#include <sferes/eval/eval.hpp>
#include <sferes/stat/best_fit.hpp>
#include <sferes/eval/parallel.hpp>
#include <sferes/modif/dummy.hpp>

using namespace sferes;
using namespace sferes::gen::evo_float;

struct Params
{
  struct evo_float
  {
    static const float cross_rate = 0.5f;
    static const float mutation_rate = 0.5f;
    static const float eta_m = 15.0f;
    static const float eta_c = 10.0f;
    static const mutation_t mutation_type = polynomial;
    static const cross_over_t cross_over_type = sbx;
  };
  struct pop
  {
    static const unsigned size = 100;
    static const unsigned nb_gen = 300;
    static const float initial_aleat = 2.0f;
    static const int dump_period = -1;
    static const float coeff = 1.1f;
    static const float keep_rate = 0.6f;    
  };
  struct parameters
  {
    static const float min = 0.0f;
    static const float max = 1.0f;
  };
};

template<typename Indiv>
float _g(const Indiv &ind)
{
  float g = 0.0f;
  assert(ind.size() == 30);
  for (size_t i = 1; i < 30; ++i)
    g += ind.data(i);
  g = 9.0f * g / 29.0f;
  g += 1.0f;
  return g;
}

SFERES_FITNESS(FitZDT2, sferes::fit::Fitness)
{
 public:
  FitZDT2() : _this(this) {}
  template<typename Indiv>
    void eval(Indiv& ind) 
  {
    assert(this == _this);
    this->_objs.resize(2);
    float f1 = ind.data(0);
    float g = _g(ind);
    float h = 1.0f - pow((f1 / g), 2.0);
    float f2 = g * h;
    this->_objs[0] = -f1;
    this->_objs[1] = -f2;
    this->_value = -f1 -f2;
  }
  FitZDT2* _this;
};


BOOST_AUTO_TEST_CASE(test_rank_simple2)
{
  srand(time(0));  
  typedef gen::EvoFloat<30, Params> gen_t;
  typedef phen::Parameters<gen_t, FitZDT2<Params>, Params> phen_t;
  typedef eval::Parallel<Params> eval_t;
  typedef boost::fusion::vector<stat::BestFit<phen_t, Params> >  stat_t;
  typedef modif::Dummy<> modifier_t;
  typedef ea::RankSimple<phen_t, eval_t, stat_t, modifier_t, Params> ea_t;
  ea_t ea;

  ea.run();
  
}
