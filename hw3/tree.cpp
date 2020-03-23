// Return the number of ways that all n2 elements of a2 appear
  // in the n1 element array a1 in the same order (though not
  // necessarily consecutively).  The empty sequence appears in a
  // sequence of length n1 in 1 way, even if n1 is 0.
  // For example, if a1 is the 7 element array
  //    10 50 40 20 50 40 30
  // then for this value of a2     the function must return
  //    10 20 40            1
  //    10 40 30            2
  //    20 10 40            0
  //    50 40 30            3

int countContains(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0) {
        return 1;
    }
    
    if (n1 <= 0) {
        return 0;
    }
    
    //10 20 20 50 40 90 40
    //10 20 40
    //
    
    //check for all tens
    //if you reach a twenty, check for all after that
        //if you reach a forty, add one since it's the end of the thing
    
    int tot = 0;
    
    if (a1[0] == a2[0]) {
        tot = tot + countContains(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
    }
   
    tot = tot + countContains(a1 + 1, n1 - 1, a2, n2);
    
    return tot;
}


  // Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

  // Rearrange the elements of the array so that all the elements
  // whose value is > separator come before all the other elements,
  // and all the elements whose value is < separator come after all
  // the other elements.  Upon return, firstNotGreater is set to the
  // index of the first element in the rearranged array that is
  // <= separator, or n if there is no such element, and firstLess is
  // set to the index of the first element that is < separator, or n
  // if there is no such element.
  // In other words, upon return from the function, the array is a
  // permutation of its original value such that
  //   * for 0 <= i < firstNotGreater, a[i] > separator
  //   * for firstNotGreater <= i < firstLess, a[i] == separator
  //   * for firstLess <= i < n, a[i] < separator
  // All the elements > separator end up in no particular order.
  // All the elements < separator end up in no particular order.
void separate(double a[], int n, double separator,
                    int& firstNotGreater, int& firstLess)
{
    if (n < 0)
    n = 0;

      // It will always be the case that just before evaluating the loop
      // condition:
      //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
      //  Every element earlier than position firstNotGreater is > separator
      //  Every element from position firstNotGreater to firstUnknown-1 is
      //    == separator
      //  Every element from firstUnknown to firstLess-1 is not known yet
      //  Every element at position firstLess or later is < separator

    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
    if (a[firstUnknown] < separator)
    {
        firstLess--;
        exchange(a[firstUnknown], a[firstLess]);
    }
    else
    {
        if (a[firstUnknown] > separator)
        {
        exchange(a[firstNotGreater], a[firstUnknown]);
        firstNotGreater++;
        }
        firstUnknown++;
    }
    }
}

void order(double a[], int n)
{
    //[5, 3, 4, 3, 2, 6, 4]
    
    //firstNotGreater = 3 (pos 4)
    //firstLess = 2 (pos 7)
    //[6, 5, 4, 4, 3, 3, 2]
    //a, n = 7,
    
    //[5, 1, 3]
    //firstNotGreater = 2
    //firstLess = 2
    //a, same
    
    if (n <= 1) {
        return;
    }
    int firstNotGreater;
    int firstLess;
    
    separate(a, n, a[n/2], firstNotGreater, firstLess);
    order(a, firstNotGreater);
    order(a + firstLess, n - firstLess);
    return;
}



