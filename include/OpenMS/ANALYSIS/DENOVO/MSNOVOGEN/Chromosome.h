// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2013.
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
// $Maintainer: Jens Allmer $
// $Authors: Jens Allmer $
// --------------------------------------------------------------------------

#ifndef OPENMS_ANALYSIS_DENOVO_MSNOVOGEN_CHROMOSOME_H
#define OPENMS_ANALYSIS_DENOVO_MSNOVOGEN_CHROMOSOME_H

#include <OpenMS/CHEMISTRY/AASequence.h>
#include <boost/shared_ptr.hpp>

namespace OpenMS
{
  /**
  * @brief A Chromosome, part of the MSNovoGen optimization approach holds
  * basic information about the sequence to be optimized and its score in
  * respect to the scoring system since recalculating a score may be expensive.
  */
  class OPENMS_DLLAPI Chromosome
  {
private:
	AASequence sequence;
	double score;
public:
    /// identifier for sorting Chromosomes such that scores are in ascending order
    const static int sortScoreAscending = 0;
    /// identifier for sorting Chromosomes such that scores are in descending order
    const static int sortScoreDescending = 1;

public:
    /// Default c'tor
    Chromosome();

    // c'tor providing all necessary information.
    Chromosome(AASequence seq, double val);

    /// Copy c'tor
    Chromosome(const Chromosome& other);

    /// Assignment operator
    Chromosome & operator=(const Chromosome& rhs);

    /// autogenerated getter for score member variable.
	double getScore() const;

	/// autogenerated setter for score member variable.
	void setScore(double score);

	/// autogenerated getter for sequence member variable.
	const AASequence & getSequence() const;

	/// autogenerated setter for sequence member variable.
	void setSequence(AASequence sequence);

	// to be used with std::sort e.g.: "std::sort(chrs.begin(),chrs.end(),Chromosome::sortScoreDesc);" in order to
	// get a sorting in a decreasing manner by score (largest score first).
	static bool sortScoreDesc(boost::shared_ptr<Chromosome> lhs, boost::shared_ptr<Chromosome> rhs);

	// to be used with std::sort e.g.: "std::sort(chrs.begin(),chrs.end(),Chromosome::sortScoreDesc);" in order to
	// get a sorting in a increasing manner by score (largest score last).
	static bool sortScoreAsc(boost::shared_ptr<Chromosome> lhs, boost::shared_ptr<Chromosome> rhs);
};
} // namespace

#endif // OPENMS_ANALYSIS_DENOVO_MSNOVOGEN_CHROMOSOME_H
