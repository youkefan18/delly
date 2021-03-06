/*
============================================================================
DELLY: Structural variant discovery by integrated PE mapping and SR analysis
============================================================================
Copyright (C) 2012 Tobias Rausch

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
============================================================================
Contact: Tobias Rausch (rausch@embl.de)
============================================================================
*/

#ifndef TAGS_H
#define TAGS_H

namespace torali {

  // Tags
  struct DeletionTag;
  struct DuplicationTag;
  struct InversionTag;
  struct TranslocationTag;
  struct InsertionTag;

  template<typename SvTag>
    struct SVType {
    };


  struct RedundancyFilterTag;
  struct NoRedundancyFilterTag;
  
  template<typename CoverageTag>
    struct CoverageType {
    };

  struct BpLevelCount;
  struct NoBpLevelCount;
  
  template<typename BpLevelTag>
    struct BpLevelType {
    };

  // F+ 0
  // F- 1
  // R+ 2
  // R- 3

  template<typename TBamRecord>
  inline int
    getStrandIndependentOrientation(TBamRecord const& al) {
    if (al.flag & BAM_FREAD1) {
      if (!(al.flag & BAM_FREVERSE)) {
	if (!(al.flag & BAM_FMREVERSE)) return (al.pos < al.mpos) ? 0 : 1;
	else return (al.pos < al.mpos) ? 2 : 3;
      } else {
	if (!(al.flag & BAM_FMREVERSE)) return (al.pos > al.mpos) ? 2 : 3;
	else return (al.pos > al.mpos) ? 0 : 1;
      }
    } else {
      if (!(al.flag & BAM_FREVERSE)) {
	if (!(al.flag & BAM_FMREVERSE)) return (al.pos < al.mpos) ? 1 : 0;
	else return (al.pos < al.mpos) ? 2 : 3;
      } else {
	if (!(al.flag & BAM_FMREVERSE)) return (al.pos > al.mpos) ? 2 : 3;
	else return (al.pos > al.mpos) ? 1 : 0;
      }
    }
  }

  //FF+ 0
  //FF- 1
  //FR+ 2
  //FR- 3
  //RF+ 4
  //RF- 5
  //RR+ 6
  //RR- 7

  template<typename TBamRecord>
  inline int
    getStrandSpecificOrientation(TBamRecord const& al) {
    if (!(al.flag  & BAM_FREVERSE)) {
      if (!(al.flag & BAM_FMREVERSE)) {
        return (al.pos < al.mpos) ? 0 : 1;
      } else {
        return (al.pos < al.mpos) ? 2 : 3;
      }
    } else {
      if (!(al.flag & BAM_FMREVERSE)) {
        return (al.pos > al.mpos) ? 4 : 5;
      } else {
        return (al.pos > al.mpos) ? 6 : 7;
      }
    }
  }


  // Deletions
  template<typename TBamRecord>
    inline int 
    _getSpanOrientation(TBamRecord const&, int const, SVType<DeletionTag>) {
    return 1;
  }

  // Duplications
  template<typename TBamRecord>
    inline int 
    _getSpanOrientation(TBamRecord const&, int const, SVType<DuplicationTag>) {
    return 1;
  }

  // Left- or right-spanning
  template<typename TBamRecord>
    inline int 
    _getSpanOrientation(TBamRecord const& al, int const defaultOrient, SVType<InversionTag>) {
    int orient = getStrandIndependentOrientation(al);
    if (al.flag & BAM_FREAD1) {
      if (defaultOrient == 0) {
	if (((orient==2) && (al.pos < al.mpos)) || ((orient == 3) && (al.pos > al.mpos))) return 1;
      } else if (defaultOrient == 1) {
	if (((orient==2) && (al.pos > al.mpos)) || ((orient == 3) && (al.pos < al.mpos))) return 1;
      } else if (defaultOrient == 2) {
	if (((orient==0) && (al.pos < al.mpos)) || ((orient == 1) && (al.pos > al.mpos))) return 1;
      } else if (defaultOrient == 3) {
	if (((orient==0) && (al.pos > al.mpos)) || ((orient == 1) && (al.pos < al.mpos))) return 1;
      }
      return 0;
    } else {
      if (defaultOrient == 0) {
	if (((orient==2) && (al.pos > al.mpos)) || ((orient == 3) && (al.pos < al.mpos))) return 1;
      } else if (defaultOrient == 1) {
	if (((orient==2) && (al.pos < al.mpos)) || ((orient == 3) && (al.pos > al.mpos))) return 1;
      } else if (defaultOrient == 2) {
	if (((orient==0) && (al.pos > al.mpos)) || ((orient == 1) && (al.pos < al.mpos))) return 1;
      } else if (defaultOrient == 3) {
	if (((orient==0) && (al.pos < al.mpos)) || ((orient == 1) && (al.pos > al.mpos))) return 1;
      }
      return 0;
    }
  }




  template<typename TBamRecord>
    inline int 
    _inOrderAssign(TBamRecord const& al, bool flipped) {
    if (!flipped) {
      if (!(al.flag & BAM_FREVERSE)) {
	if (!(al.flag & BAM_FMREVERSE)) {
	  return (al.flag & BAM_FREAD1) ? 0 : 1;
	} else {
	  return 2;
	}
      } else {
	if (!(al.flag & BAM_FMREVERSE)) {
	  return 3;
	} else {
	  return (al.flag & BAM_FREAD1) ? 1 : 0;
	}
      }
    } else {
      if (!(al.flag & BAM_FREVERSE)) {
	if (!(al.flag & BAM_FMREVERSE)) {
	  return 2;
	} else {
	  return (al.flag & BAM_FREAD1) ? 0 : 1;
	}
      } else {
	if (!(al.flag & BAM_FMREVERSE)) {
	  return (al.flag & BAM_FREAD1) ? 1 : 0;
	} else {
	  return 3;
	}
      }
    }
  }


  template<typename TBamRecord>
    inline int 
    _getSpanOrientation(TBamRecord const& al, int const defaultOrient, SVType<TranslocationTag>) {
    int orient = getStrandIndependentOrientation(al);
    bool flipped = ( ((defaultOrient<2) && (orient>=2)) || ((defaultOrient>=2) && (orient<2)) );
    bool inOrder = (_inOrderAssign(al, flipped) == defaultOrient);
    if (flipped) {
      if (inOrder) return 0;
      else return 1;
    } else {
      if (inOrder) return 2;
      else return 3;
    }
  }

  // Insertion
  template<typename TBamRecord>
    inline int 
    _getSpanOrientation(TBamRecord const&, int const, SVType<InsertionTag>) {
    return 1;
  }

  // Reduced structural variant record for cov
  struct CovRecord {
    int32_t chr;
    int32_t svStart;
    int32_t svEnd;
    uint32_t id;

    CovRecord() {}
    CovRecord(int32_t const c, int32_t const s, int32_t const e) : chr(c), svStart(s), svEnd(e) {}
  };

  // Structural variant record
  struct StructuralVariantRecord {
    int svStartBeg;
    int svStartEnd;
    int svEndBeg;
    int svEndEnd;
    int svStart;
    int svEnd;
    int peSupport;
    int srSupport;
    int wiggle;
    int insLen;
    double srAlignQuality;
    unsigned int id;
    unsigned int controlID;
    bool precise;
    int ct;
    uint8_t peMapQuality;
    int32_t chr;
    int32_t chr2;
    std::string consensus;

    StructuralVariantRecord() {}
  StructuralVariantRecord(int32_t const c, int const s, int const e) : svStart(s), svEnd(e), chr(c) {}
  };

  template<typename TSV>
  struct SortSVs : public std::binary_function<TSV, TSV, bool>
  {
    inline bool operator()(TSV const& sv1, TSV const& sv2) {
      return ((sv1.chr<sv2.chr) || ((sv1.chr==sv2.chr) && (sv1.svStart<sv2.svStart)) || ((sv1.chr==sv2.chr) && (sv1.svStart==sv2.svStart) && (sv1.svEnd<sv2.svEnd)));
    }
  };



  // SV Paired-end checks

  // Deletions, insertions, duplications and inversions
  template<typename TPos, typename TTag>
    inline TPos
    _minCoord(TPos const position, TPos const matePosition, SVType<TTag>) {
    return std::min(position, matePosition);
  }

  // Translocations
  template<typename TPos>
    inline TPos
    _minCoord(TPos const position, TPos const, SVType<TranslocationTag>) {
    return position;
  }

  // Deletions, insertions, duplications and inversions
  template<typename TPos, typename TTag>
    inline TPos
    _maxCoord(TPos const position, TPos const matePosition, SVType<TTag>) {
    return std::max(position, matePosition);
  }

  // Translocations
  template<typename TPos>
    inline TPos
    _maxCoord(TPos const, TPos const matePosition, SVType<TranslocationTag>) {
    return matePosition;
  }

  // Deletions, duplications and inversions
  template<typename TQualities, typename TTag>
    inline void
    _resetQualities(TQualities& qualities, SVType<TTag>) {
    qualities.clear();
  }

  // Translocations
  template<typename TQualities>
    inline void
    _resetQualities(TQualities&, SVType<TranslocationTag>) {
    // Nop
  }

  // Deletions
  template<typename TRef, typename TPos>
    inline bool
    _mappingPos(TRef const refID, TRef const mateRefID, TPos const position, TPos const matePosition, SVType<DeletionTag>) {
    return ((refID!=mateRefID) || (position==matePosition));
  }

  // Duplications
  template<typename TRef, typename TPos>
    inline bool
    _mappingPos(TRef const refID, TRef const mateRefID, TPos const position, TPos const matePosition, SVType<DuplicationTag>) {
    return ((refID!=mateRefID) || (std::abs(position - matePosition) < 100 ));
  }

  // Inversions
  template<typename TRef, typename TPos>
    inline bool
    _mappingPos(TRef const refID, TRef const mateRefID, TPos const position, TPos const matePosition, SVType<InversionTag>) {
    return ((refID!=mateRefID) || (position==matePosition));
  }

  // Translocations
  template<typename TRef, typename TPos>
    inline bool
    _mappingPos(TRef const refID, TRef const mateRefID, TPos const, TPos const, SVType<TranslocationTag>) {
    return (refID==mateRefID);
  }

  // Insertion
  template<typename TRef, typename TPos>
    inline bool
    _mappingPos(TRef const refID, TRef const mateRefID, TPos const position, TPos const matePosition, SVType<InsertionTag>) {
    return ((refID!=mateRefID) || (position==matePosition));
  }

  // Deletions, duplications and inversions
  template<typename TRef, typename TPos, typename TTag>
    inline bool
    _mappingPosGeno(TRef const refID, TRef const mateRefID, TPos const position, TPos const matePosition, SVType<TTag> svType) {
    return _mappingPos(refID, mateRefID, position, matePosition, svType);
  }

  // Translocations
  template<typename TRef, typename TPos>
    inline bool
    _mappingPosGeno(TRef const refID, TRef const mateRefID, TPos const position, TPos const matePosition, SVType<TranslocationTag>) {
    return ((refID==mateRefID) && (position==matePosition));
  }

  // Deletions, duplications and inversions
  template<typename TRef, typename TPos, typename TTag>
    inline bool
    _firstPairObs(TRef const, TRef const, TPos const position, TPos const matePosition, SVType<TTag>) {
    return (position<matePosition);
  }

  // Translocations
  template<typename TRef, typename TPos>
    inline bool
    _firstPairObs(TRef const refID, TRef const mateRefID, TPos const position, TPos const matePosition, SVType<TranslocationTag>) {
    return ((refID<mateRefID) || ((refID==mateRefID) && (position<matePosition)));
  }

  // Deletions
  template<typename TISize, typename TLibInfo>
    inline bool
    _acceptedInsertSize(TLibInfo& libInfo, TISize const iSize, SVType<DeletionTag>) {
    return (libInfo.maxNormalISize > iSize);
  }

  // Insertions
  template<typename TISize, typename TLibInfo>
    inline bool
    _acceptedInsertSize(TLibInfo& libInfo, TISize const iSize, SVType<InsertionTag>) {
    return (libInfo.minNormalISize <= iSize);
  }

  // Duplications
  template<typename TISize, typename TLibInfo>
    inline bool
    _acceptedInsertSize(TLibInfo& libInfo, TISize const iSize, SVType<DuplicationTag>) {
    // Exclude the chimeras in mate-pair libraries
    return !((libInfo.median<1000) || ((libInfo.median>=1000) && (iSize >=1000)));
  }

  // Other SV Types
  template<typename TISize, typename TLibInfo, typename TTag>
    inline bool
    _acceptedInsertSize(TLibInfo&, TISize const, SVType<TTag>) {
    return false;
  }

  // Deletions
  template<typename TOrientation>
  inline bool
    _acceptedOrientation(TOrientation const def, TOrientation const lib, SVType<DeletionTag>) {
    return (def != lib);
  }

  // Insertions
  template<typename TOrientation>
  inline bool
    _acceptedOrientation(TOrientation const def, TOrientation const lib, SVType<InsertionTag>) {
    return (def != lib);
  }

  // Duplications
  template<typename TOrientation>
    inline bool
    _acceptedOrientation(TOrientation const def, TOrientation const lib, SVType<DuplicationTag>) {
    if (def==0) return (lib != 1);
    else if (def==1) return (lib!=0);
    else if (def==2) return (lib!=3);
    else if (def==3) return (lib!=2);
    else return true;
  }

  // Inversion
  template<typename TOrientation>
    inline bool
    _acceptedOrientation(TOrientation const def, TOrientation const lib, SVType<InversionTag>) {
    return (((def<=1) && (lib!=2) && (lib!=3)) || ((def>=2) && (lib!=0) && (lib!=1)));
  }

  // Other SV Types
  template<typename TOrientation, typename TTag>
    inline bool
    _acceptedOrientation(TOrientation const, TOrientation const, SVType<TTag>) {
    return false;
  }


  // Deletions
  template<typename TSize, typename TISize>
    inline bool
    _pairsDisagree(TSize const pair1Min, TSize const pair1Max, TSize const pair1ReadLength, TISize const pair1maxNormalISize, TSize const pair2Min, TSize const pair2Max, TSize const pair2ReadLength, TISize const pair2maxNormalISize, int const, int const, SVType<DeletionTag>) {
    //std::cout << pair1Min << ',' << pair1Max << ',' << pair1ReadLength << ',' << pair1maxNormalISize << ',' << pair2Min << ',' << pair2Max << ',' << pair2ReadLength << ',' << pair2maxNormalISize << std::endl;
    if ((pair2Min + pair2ReadLength - pair1Min) > pair1maxNormalISize) return true;
    if ((pair2Max < pair1Max) && ((pair1Max + pair1ReadLength - pair2Max) > pair1maxNormalISize)) return true;
    if ((pair2Max >= pair1Max) && ((pair2Max + pair2ReadLength - pair1Max) > pair2maxNormalISize)) return true;
    if ((pair1Max < pair2Min) || (pair2Max < pair1Min)) return true;
    return false;
  }

  // Insertions
  template<typename TSize, typename TISize>
    inline bool
    _pairsDisagree(TSize const pair1Min, TSize const pair1Max, TSize const pair1ReadLength, TISize const pair1maxNormalISize, TSize const pair2Min, TSize const pair2Max, TSize const pair2ReadLength, TISize const pair2maxNormalISize, int const, int const, SVType<InsertionTag>) {
    //std::cout << pair1Min << ',' << pair1Max << ',' << pair1ReadLength << ',' << pair1maxNormalISize << ',' << pair2Min << ',' << pair2Max << ',' << pair2ReadLength << ',' << pair2maxNormalISize << std::endl;
    if ((pair2Min + pair2ReadLength - pair1Min) > pair1maxNormalISize) return true;
    if ((pair2Max < pair1Max) && ((pair1Max + pair1ReadLength - pair2Max) > pair1maxNormalISize)) return true;
    if ((pair2Max >= pair1Max) && ((pair2Max + pair2ReadLength - pair1Max) > pair2maxNormalISize)) return true;
    if ((pair1Max < pair2Min) || (pair2Max < pair1Min)) return true;
    return false;
  }


  // Duplications
  template<typename TSize, typename TISize>
    inline bool
    _pairsDisagree(TSize const pair1Min, TSize const pair1Max, TSize const pair1ReadLength, TISize const pair1maxNormalISize, TSize const pair2Min, TSize const pair2Max, TSize const pair2ReadLength, TISize const pair2maxNormalISize, int const, int const, SVType<DuplicationTag>) {
    if ((pair2Min + pair2ReadLength - pair1Min) > pair2maxNormalISize) return true;
    if ((pair2Max < pair1Max) && ((pair1Max + pair1ReadLength - pair2Max) > pair2maxNormalISize)) return true;
    if ((pair2Max >= pair1Max) && ((pair2Max + pair2ReadLength - pair1Max) > pair1maxNormalISize)) return true;
    return false;
  }


  // Inversions
  template<typename TSize, typename TISize>
    inline bool
    _pairsDisagree(TSize const pair1Min, TSize const pair1Max, TSize const pair1ReadLength, TISize const pair1maxNormalISize, TSize const pair2Min, TSize const pair2Max, TSize const pair2ReadLength, TISize const pair2maxNormalISize, int const ct1, int const ct2, SVType<InversionTag>) {
    // Do both pairs support the same inversion type (left- or right-spanning)
    if (ct1 != ct2) return true;
    if (ct1==1) {
      // Left-spanning inversions
      if ((pair2Min + pair2ReadLength - pair1Min) > pair1maxNormalISize) return true;
      if ((pair2Max < pair1Max) && ((pair1Max + pair1ReadLength - pair2Max) > pair2maxNormalISize)) return true;
      if ((pair2Max >= pair1Max) && ((pair2Max + pair2ReadLength - pair1Max) > pair1maxNormalISize)) return true;
    } else {
      // Right-spanning inversions
      if ((pair2Min + pair2ReadLength - pair1Min) > pair2maxNormalISize) return true;
      if ((pair2Max < pair1Max) && ((pair1Max + pair1ReadLength - pair2Max) > pair1maxNormalISize)) return true;
      if ((pair2Max >= pair1Max) && ((pair2Max + pair2ReadLength - pair1Max) > pair2maxNormalISize)) return true;
    }
    return false;
  }


  // Translocations
  template<typename TSize, typename TISize>
    inline bool
    _pairsDisagree(TSize const pair1Min, TSize const pair1Max, TSize const pair1ReadLength, TISize const pair1maxNormalISize, TSize const pair2Min, TSize const pair2Max, TSize const pair2ReadLength, TISize const pair2maxNormalISize, int const ct1, int const ct2, SVType<TranslocationTag>) {
    // Do both pairs support the same translocation type
    if (ct1 != ct2) return true;

    // Check read offsets
    if (ct1%2==0) {
      if ((pair2Min + pair2ReadLength - pair1Min) > pair1maxNormalISize) return true;
      if (ct1>=2) {
	if (pair2Max < pair1Max) {
	  if ((pair1Max + pair1ReadLength - pair2Max) > pair1maxNormalISize) return true;
	} else {
	  if ((pair2Max + pair2ReadLength - pair1Max) > pair2maxNormalISize) return true;
	}
      } else {
	if (pair2Max < pair1Max) {
	  if ((pair1Max + pair1ReadLength - pair2Max) > pair2maxNormalISize) return true;
	} else {
	  if ((pair2Max + pair2ReadLength - pair1Max) > pair1maxNormalISize) return true;
	}
      }
    } else {
      if ((pair2Min + pair2ReadLength - pair1Min) > pair2maxNormalISize) return true;
      if (ct1>=2) {
	if (pair2Max < pair1Max) {
	  if ((pair1Max + pair1ReadLength - pair2Max) > pair2maxNormalISize) return true;
	} else {
	  if ((pair2Max + pair2ReadLength - pair1Max) > pair1maxNormalISize) return true;
	}
      } else {
	if (pair2Max < pair1Max) {
	  if ((pair1Max + pair1ReadLength - pair2Max) > pair1maxNormalISize) return true;
	} else {
	  if ((pair2Max + pair2ReadLength - pair1Max) > pair2maxNormalISize) return true;
	}
      }
    }
    return false;
  }





// Deletions
template<typename TSeq, typename TSVRecord, typename TRef>
inline std::string
_getSVRef(TSeq const* const ref, TSVRecord const& svRec, TRef const, SVType<DeletionTag>) {
  return boost::to_upper_copy(std::string(ref + svRec.svStartBeg, ref + svRec.svStartEnd)) + boost::to_upper_copy(std::string(ref + svRec.svEndBeg, ref + svRec.svEndEnd));
}

// Insertions
template<typename TSeq, typename TSVRecord, typename TRef>
inline std::string
_getSVRef(TSeq const* const ref, TSVRecord const& svRec, TRef const, SVType<InsertionTag>) {
  return boost::to_upper_copy(std::string(ref + svRec.svStartBeg, ref + svRec.svEndEnd));
}

// Duplications
template<typename TSeq, typename TSVRecord, typename TRef>
inline std::string
_getSVRef(TSeq const* const ref, TSVRecord const& svRec, TRef const, SVType<DuplicationTag>) {
  return boost::to_upper_copy(std::string(ref + svRec.svEndBeg, ref + svRec.svEndEnd)) + boost::to_upper_copy(std::string(ref + svRec.svStartBeg, ref + svRec.svStartEnd));
}

// Inversions
template<typename TSeq, typename TSVRecord, typename TRef>
inline std::string
_getSVRef(TSeq const* const ref, TSVRecord const& svRec, TRef const, SVType<InversionTag>) {
  if (svRec.ct==1) {
    std::string strEnd=boost::to_upper_copy(std::string(ref + svRec.svEndBeg, ref + svRec.svEndEnd));
    std::string strRevComp=strEnd;
    std::string::reverse_iterator itR = strEnd.rbegin();
    std::string::reverse_iterator itREnd = strEnd.rend();
    for(unsigned int i = 0; itR!=itREnd; ++itR, ++i) {
      switch (*itR) {
      case 'A': strRevComp[i]='T'; break;
      case 'C': strRevComp[i]='G'; break;
      case 'G': strRevComp[i]='C'; break;
      case 'T': strRevComp[i]='A'; break;
      case 'N': strRevComp[i]='N'; break;
      default: break;
      }
    }
    return boost::to_upper_copy(std::string(ref + svRec.svStartBeg, ref + svRec.svStartEnd)) + strRevComp;
  } else {
    std::string strStart=boost::to_upper_copy(std::string(ref + svRec.svStartBeg, ref + svRec.svStartEnd));
    std::string strRevComp=strStart;
    std::string::reverse_iterator itR = strStart.rbegin();
    std::string::reverse_iterator itREnd = strStart.rend();
    for(unsigned int i = 0; itR!=itREnd; ++itR, ++i) {
      switch (*itR) {
      case 'A': strRevComp[i]='T'; break;
      case 'C': strRevComp[i]='G'; break;
      case 'G': strRevComp[i]='C'; break;
      case 'T': strRevComp[i]='A'; break;
      case 'N': strRevComp[i]='N'; break;
      default: break;
      }
    }
    return strRevComp + boost::to_upper_copy(std::string(ref + svRec.svEndBeg, ref + svRec.svEndEnd));
  }
}

// Translocations
template<typename TSeq, typename TSVRecord, typename TRef>
inline std::string
_getSVRef(TSeq const* const ref, TSVRecord const& svRec, TRef const refIndex, SVType<TranslocationTag>) {
  if (svRec.chr==refIndex) {
    if ((svRec.ct==0) || (svRec.ct==2)) return boost::to_upper_copy(std::string(ref + svRec.svStartBeg, ref + svRec.svStartEnd)) + svRec.consensus;
    else if (svRec.ct==1) return svRec.consensus + boost::to_upper_copy(std::string(ref + svRec.svStartBeg, ref + svRec.svStartEnd));
    else {
      std::string strEnd=boost::to_upper_copy(std::string(ref + svRec.svStartBeg, ref + svRec.svStartEnd));
      std::string refPart=strEnd;
      std::string::reverse_iterator itR = strEnd.rbegin();
      std::string::reverse_iterator itREnd = strEnd.rend();
      for(unsigned int i = 0; itR!=itREnd; ++itR, ++i) {
	switch (*itR) {
	case 'A': refPart[i]='T'; break;
	case 'C': refPart[i]='G'; break;
	case 'G': refPart[i]='C'; break;
	case 'T': refPart[i]='A'; break;
	case 'N': refPart[i]='N'; break;
	default: break;
	}
      }
      return svRec.consensus + refPart;
    }
  } else {
    // chr2
    if (svRec.ct==2) return boost::to_upper_copy(std::string(ref + svRec.svEndBeg, ref + svRec.svEndEnd));
    else {
      std::string strEnd=boost::to_upper_copy(std::string(ref + svRec.svEndBeg, ref + svRec.svEndEnd));
      std::string refPart=strEnd;
      std::string::reverse_iterator itR = strEnd.rbegin();
      std::string::reverse_iterator itREnd = strEnd.rend();
      for(unsigned int i = 0; itR!=itREnd; ++itR, ++i) {
	switch (*itR) {
	case 'A': refPart[i]='T'; break;
	case 'C': refPart[i]='G'; break;
	case 'G': refPart[i]='C'; break;
	case 'T': refPart[i]='A'; break;
	case 'N': refPart[i]='N'; break;
	default: break;
	}
      }
      return refPart;
    }
  }
  return "";
}



}

#endif
