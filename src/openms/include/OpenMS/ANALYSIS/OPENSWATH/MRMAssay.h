// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2017.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: George Rosenberger $
// $Authors: George Rosenberger $
// --------------------------------------------------------------------------

#pragma once

#include <OpenMS/ANALYSIS/TARGETED/TargetedExperiment.h>
#include <OpenMS/CONCEPT/ProgressLogger.h>
#include <OpenMS/CONCEPT/LogStream.h>
#include <OpenMS/ANALYSIS/OPENSWATH/MRMIonSeries.h>
#include <OpenMS/CHEMISTRY/ModificationsDB.h>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

// #define DEBUG_MRMASSAY

namespace OpenMS
{
  /**
    @brief Generate assays from a TargetedExperiment

    Will generate assays from a raw, unfiltered TargetedExperiment, as can be
    generated by TargetedFileConverter.

    Transitions can be selected according to a set of rules, as described in
    Schubert et al., 2015 (PMID: 25675208).

    In addition, unique ion signature (UIS) (Sherman et al., 2009 (PMID: 19556279))
    transitions can be generated based on empirically observed or in silico generated ion series.

  */
  class OPENMS_DLLAPI MRMAssay :
    public ProgressLogger
  {

public:
    //@{
    /// Constructor
    MRMAssay(); // empty, no members

    /// Destructor
    ~MRMAssay();
    //@}

    typedef std::vector<OpenMS::TargetedExperiment::Protein> ProteinVectorType;
    typedef std::vector<OpenMS::TargetedExperiment::Peptide> PeptideVectorType;
    typedef std::vector<OpenMS::ReactionMonitoringTransition> TransitionVectorType;

    typedef std::map<String, std::vector<const ReactionMonitoringTransition*> > PeptideTransitionMapType;

    typedef boost::unordered_map<size_t, boost::unordered_map<String, std::set<std::string> > > SequenceMapT;
    typedef boost::unordered_map<size_t, boost::unordered_map<String, std::vector<std::pair<double, std::string> > > > IonMapT;
    typedef std::map<String, std::vector<std::pair<std::string, double> > > PeptideMapT;
    typedef boost::unordered_map<String, String> SequenceMap_T;

    typedef boost::unordered_map<String, TargetedExperiment::Peptide> TargetDecoyMapT;

    /**
      @brief Annotates and filters transitions in a TargetedExperiment

      @param exp the input, unfiltered transitions
      @param precursor_mz_threshold the precursor m/z threshold in Th for annotation
      @param product_mz_threshold the product m/z threshold in Th for annotation
      @param fragment_types the fragment types to consider for annotation
      @param fragment_charges the fragment charges to consider for annotation
      @param enable_specific_losses whether specific neutral losses should be considered
      @param enable_unspecific_losses whether unspecific neutral losses (H2O1, H3N1, C1H2N2, C1H2N1O1) should be considered
      @param round_decPow round product m/z values to decimal power (default: -4)

    */
    void reannotateTransitions(OpenMS::TargetedExperiment& exp, double precursor_mz_threshold, double product_mz_threshold, std::vector<String> fragment_types, std::vector<size_t> fragment_charges, bool enable_specific_losses, bool enable_unspecific_losses, int round_decPow = -4);

    /**
      @brief Restrict and filter transitions in a TargetedExperiment

      @param exp the input, unfiltered transitions
      @param lower_mz_limit the lower product m/z limit in Th
      @param upper_mz_limit the upper product m/z limit in Th
      @param swathes the swath window settings (to exclude fragment ions falling
      into the precursor isolation window)

    */
    void restrictTransitions(OpenMS::TargetedExperiment& exp, double lower_mz_limit, double upper_mz_limit, std::vector<std::pair<double, double> > swathes);

    /**
      @brief Select detecting fragment ions

      @param exp the input, unfiltered transitions
      @param min_transitions the minimum number of transitions required per assay
      @param max_transitions the maximum number of transitions required per assay

    */
    void detectingTransitions(OpenMS::TargetedExperiment& exp, int min_transitions, int max_transitions);

    /**
      @brief Annotate UIS / site-specific transitions

      @param exp the input, unfiltered transitions
      @param fragment_types the fragment types to consider for annotation
      @param fragment_charges the fragment charges to consider for annotation
      @param enable_specific_losses whether specific neutral losses should be considered
      @param enable_unspecific_losses whether unspecific neutral losses (H2O1, H3N1, C1H2N2, C1H2N1O1) should be considered
      @param enable_ms2_precursors whether MS2 precursors should be considered
      @param mz_threshold the product m/z threshold in Th for annotation
      @param swathes the swath window settings (to exclude fragment ions falling
      @param round_decPow round product m/z values to decimal power (default: -4)
      @param max_num_alternative_localizations maximum number of allowed peptide sequence permutations
      @param shuffle_seed set seed for shuffle (-1: select seed based on time)
      @param disable_decoy_transitions whether to disable generation of decoy UIS transitions
    */
    void uisTransitions(OpenMS::TargetedExperiment& exp,
                        std::vector<String> fragment_types,
                        std::vector<size_t> fragment_charges,
                        bool enable_specific_losses,
                        bool enable_unspecific_losses,
                        bool enable_ms2_precursors,
                        double mz_threshold,
                        std::vector<std::pair<double, double> > swathes,
                        int round_decPow = -4,
                        size_t max_num_alternative_localizations = 20,
                        int shuffle_seed = -1,
                        bool disable_decoy_transitions = false);

protected:
    /**
      @brief Check whether fragment ion are unique ion signatures in vector within threshold and return matching peptidoforms

      @param fragment_ion the queried fragment ion
      @param ions a vector of pairs of fragment ion m/z and peptide sequences which could interfere with fragment_ion
      @param mz_threshold the threshold within which to search for interferences

      @value a vector of strings containing all peptidoforms with which fragment_ion overlaps
    */
    std::vector<std::string> getMatchingPeptidoforms_(const double fragment_ion,
                                                      const std::vector<std::pair<double, std::string> >& ions,
                                                      const double mz_threshold);

    /**
      @brief Get swath index (precursor isolation window ordinal) for a particular precursor

      @param swathes the swath window settings
      @param precursor_mz the query precursor m/z

      @value index of swath where precursor_mz falls into
    */
    int getSwath_(const std::vector<std::pair<double, double> >& swathes, const double precursor_mz);

    /**
      @brief Check whether the product m/z of a transition falls into the precursor isolation window

      @param swathes the swath window settings
      @param precursor_mz the query precursor m/z
      @param product_mz the query product m/z

      @value whether product m/z falls into precursor isolation window
    */
    bool isInSwath_(const std::vector<std::pair<double, double> >& swathes, const double precursor_mz, const double product_mz);

    /**
      @brief Generates random peptide sequence

      @param sequence_size length of peptide sequence
      @param pseudoRNG a Boost pseudo RNG

      @value random peptide sequence
    */
    std::string getRandomSequence_(size_t sequence_size, boost::variate_generator<boost::mt19937&, boost::uniform_int<> > pseudoRNG);

    /**
      @brief Computes all N choose K combinations

      @param n vector of N indices
      @param k number of K

      @value a vector of all N index combinations
    */
    std::vector<std::vector<size_t> > nchoosekcombinations_(std::vector<size_t> n, size_t k);

    /**
      @brief Generate modified peptide forms based on all possible combinations

      @param sequences template AASequences

      @param mods_combs all possible combinations (e.g. from nchoosekcombinations)

      @param modification String of the modification

      @value a vector of all modified peptides.
    */
    std::vector<OpenMS::AASequence> addModificationsSequences_(std::vector<OpenMS::AASequence> sequences, std::vector<std::vector<size_t> > mods_combs, OpenMS::String modification);

    /**
      @brief Generate alternative modified peptide forms according to ModificationsDB

      @details An input peptide sequence containing modifications is used as template to generate
      all modification-carrying residue permutations (n choose k possibilities) that are
      physicochemically possible according to ModificationsDB.

      @param sequence template AASequence

      @value a vector of all alternative modified peptides.
    */
    std::vector<OpenMS::AASequence> combineModifications_(OpenMS::AASequence sequence);

    /**
      @brief Generate alternative modified peptide forms according to ModificationsDB

      @details An input peptide sequence containing modifications is used as template to generate
      all modification-carrying residue permutations (n choose k possibilities) that are
      physicochemically possible according to ModificationsDB. Instead of the target sequence, the
      permutations are transferred to the decoy sequence that might contain additional modifiable
      residues. E.g. target sequence SAS(Phospho)K could result in [SAS(Phospho)K, S(Phospho)ASK]
      but the responding set of the decoy sequence SSS(Phospho)K would be [SSS(Phospho)K, S(Phospho)SSK].

      @param sequence template AASequence

      @param decoy_sequence template decoy AASequence

      @value a vector of all alternative modified peptides.

    */
    std::vector<OpenMS::AASequence> combineDecoyModifications_(OpenMS::AASequence sequence, OpenMS::AASequence decoy_sequence);

    /**
      @brief Generate target in silico map

      @details Used internally by MRMAssay::uisTransitions

    */
    void generateTargetInSilicoMap_(OpenMS::TargetedExperiment& exp,
                                    std::vector<String> fragment_types,
                                    std::vector<size_t> fragment_charges,
                                    bool enable_specific_losses,
                                    bool enable_unspecific_losses,
                                    bool enable_ms2_precursors,
                                    std::vector<std::pair<double, double> > swathes,
                                    int round_decPow,
                                    size_t max_num_alternative_localizations,
                                    SequenceMapT& TargetSequenceMap,
                                    IonMapT& TargetIonMap,
                                    PeptideMapT& TargetPeptideMap);

    /**
      @brief Generate decoy sequences

      @details Used internally by MRMAssay::uisTransitions

    */
    void generateDecoySequences_(boost::unordered_map<size_t,
                                 boost::unordered_map<String, std::set<std::string> > >& TargetSequenceMap,
                                 boost::unordered_map<String, String>& DecoySequenceMap, 
                                 int shuffle_seed);

    /**
      @brief Generate decoy in silico map

      @details Used internally by MRMAssay::uisTransitions

    */
    void generateDecoyInSilicoMap_(OpenMS::TargetedExperiment& exp,
                                   std::vector<String> fragment_types, std::vector<size_t> fragment_charges,
                                   bool enable_specific_losses,
                                   bool enable_unspecific_losses,
                                   bool enable_ms2_precursors,
                                   std::vector<std::pair<double, double> > swathes,
                                   int round_decPow,
                                   TargetDecoyMapT& TargetDecoyMap,
                                   PeptideMapT& TargetPeptideMap,
                                   boost::unordered_map<String, String>& DecoySequenceMap,
                                   IonMapT& DecoyIonMap,
                                   PeptideMapT& DecoyPeptideMap);

    /**
      @brief Generate target assays

      @details Used internally by MRMAssay::uisTransitions

    */
   void generateTargetAssays_(OpenMS::TargetedExperiment& exp,
                              TransitionVectorType& transitions, double mz_threshold,
                              std::vector<std::pair<double, double> > swathes,
                              int round_decPow,
                              PeptideMapT& TargetPeptideMap,
                              IonMapT& TargetIonMap);

    /**
      @brief Generate decoy assays

      @details Used internally by MRMAssay::uisTransitions

      TODO: why make copy of IonMapT ?

    */
   void generateDecoyAssays_(OpenMS::TargetedExperiment& exp,
                             TransitionVectorType& transitions,
                             double mz_threshold,
                             std::vector<std::pair<double, double> > swathes,
                             int round_decPow,
                             PeptideMapT& DecoyPeptideMap,
                             TargetDecoyMapT& TargetDecoyMap, 
                             IonMapT DecoyIonMap, 
                             IonMapT TargetIonMap);

  };
}

