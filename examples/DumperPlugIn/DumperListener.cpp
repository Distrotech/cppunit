// //////////////////////////////////////////////////////////////////////////
// Implementation file DumperListener.cpp for class DumperListener
// (c)Copyright 2000, Baptiste Lepilleur.
// Created: 2002/04/19
// //////////////////////////////////////////////////////////////////////////
#include <cppunit/Test.h>
#include <iostream>
#include "DumperListener.h"

DumperListener::DumperListener( bool flatten )
    : m_flatten( flatten )
    , m_suiteCount( 0 )
    , m_testCount( 0 )
    , m_suiteWithTestCount( 0 )
{
}


DumperListener::~DumperListener()
{
}


void 
DumperListener::startTest( CppUnit::Test *test )
{
  printPath( test, false );
  ++m_testCount;
}


void 
DumperListener::endTest( CppUnit::Test *test )
{
  m_path.up();
  if ( !m_suiteHasTest.empty() )
  {
    m_suiteHasTest.pop();
    m_suiteHasTest.push( true );
  }
}


void 
DumperListener::startSuite( CppUnit::Test *suite )
{
  printPath( suite, true );
  ++m_suiteCount;
  m_suiteHasTest.push( false );
}


void 
DumperListener::endSuite( CppUnit::Test *suite )
{
  m_path.up();
  if ( m_suiteHasTest.top() )
    ++m_suiteWithTestCount;
  m_suiteHasTest.pop();
}


void 
DumperListener::endTestRun( CppUnit::Test *test, 
                            CppUnit::TestResult *eventManager )
{
  double average = 0;
  if ( m_suiteWithTestCount > 0 )
    average = double(m_testCount) / m_suiteWithTestCount;

  std::cout << "Statistics: "  <<  m_testCount  <<  " test cases, "
            << m_suiteCount << " suites, "
            << average << " test cases / suite with test cases"
            << std::endl;
}


void 
DumperListener::printPath( CppUnit::Test *test, 
                           bool isSuite )
{
  m_path.add( test );

  if ( m_flatten )
    printFlattenedPath( isSuite );
  else
    printIndentedPathChild();
}


void 
DumperListener::printFlattenedPath( bool isSuite )
{
  std::string path = m_path.toString();
  if ( isSuite )
    path += "/";
  std::cout  <<  path  <<  std::endl;
}


void 
DumperListener::printIndentedPathChild()
{
  std::string indent = makeIndentString( m_path.getTestCount() -1 );
  std::cout  <<  indent  <<  m_path.getChildTest()->getName()  <<  std::endl;
}


std::string 
DumperListener::makeIndentString( int indentLevel )
{
  std::string indent;
  for ( int parentIndent =0; parentIndent < indentLevel-1; ++parentIndent )
    indent += "|  ";
  
  if ( indentLevel > 0 )
    indent += "+--";

  return indent;
}