#include "truthTable.h"


const char TruthToChar[4] = {'0', '1', '-', '?'};

int TruthTable::setNumVars(unsigned nV)
{
  numVars = nV;
  return 0;
}

int TruthTable::addEntry(const string &newEntry)
{
  if (newEntry.length() != numVars)
  {
    cout << "ERROR in truthTable::addEntry() - new entry has length "
         << newEntry.length() << " vs. numVars = " << numVars << endl;
    return -1;
  }
  
  vector<truthType> row(newEntry.length());
  
  for (unsigned i = 0; i < newEntry.length(); ++i)
  {
    switch(newEntry[i])
    {
      case '0': row[i] = ZERO; break;
      case '1': row[i] = ONE;  break;
      case '-': row[i] = DC;   break;
      default:
        cout << "ERROR in truthTable::addEntry() - invalid character "
             << newEntry[i] << endl;
        return -1;
    }
  }
  logic.push_back(row);
  
  return 0;
}

int TruthTable::isCovered(const vector<truthType> &entry)
{
    // Debug
    //cout << "isCovered input size: " << entry.size() << endl;
    //for (unsigned i = 0; i < entry.size(); i++) {
    //    cout << entry[i] << " ";
    //}
    //cout << endl;

    // For each truth table entry, we check if it covers the given input
    // combination.
    for (unsigned i = 0; i < logic.size(); i++) {
        const vector<truthType> &have = logic[i];

        if (isCoveredBy(have, entry))   // return True if covered by any of the entries
            return 1;
    }

    return 0;
}


int TruthTable::isCoveredBy(const vector<truthType> &have, const vector<truthType> &target)
{
    if (have.size() != target.size()) {
        return 0;
    }

    for (unsigned i = 0; i < have.size(); i++) {
        if (have[i] == DC)
            continue;
        else if (have[i] == target[i])
            continue;
        else
            return 0;
    }

    return 1;
}


unsigned TruthTable::getNumEntries()
{
  return logic.size();
}

unsigned TruthTable::getNumVars()
{
  return numVars;
}

int TruthTable::print()
{
  for (unsigned i = 0; i < logic.size(); ++i)
  {
    for (unsigned j = 0; j < logic[i].size(); ++j)
    {
      switch(logic[i][j])
      {
        case ZERO: cout << "0"; break;
        case ONE:  cout << "1"; break;
        case DC:   cout << "-"; break;
        default:
          cout << "ERROR in truthTable::printTable() - invalid character "
               << logic[i][j] << endl;
          return -1;
      }
    }
    cout << " 1" << endl;
  }
  return 0;
}

int TruthTable::clear()
{
  logic.clear();
  return 0;
}

