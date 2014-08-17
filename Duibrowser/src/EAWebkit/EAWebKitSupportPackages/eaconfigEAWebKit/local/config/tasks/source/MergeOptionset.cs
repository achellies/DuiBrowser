/*
Copyright (C) 2005,2009 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//-----------------------------------------------------------------------------
// mergeoption.cs
//
// NAnt custom task which merges options  from a NAnt code-build script.
// This differs from the built-in Framework mergeoption in that it is using C#
// instead of nAnt tasks to do the work in order to increase performance
//
// Written by:
//		Alex Van der Star (avdstar@ea.com) - 2005
//
//-----------------------------------------------------------------------------
using System;
using System.Text;
using System.Collections;
using System.Xml ;


using NAnt.Core;
using NAnt.Core.Attributes;


namespace EA.MergeOptionset
{
	[TaskName("MergeOptionset")]
	public class mergeoptionset : Task
	{
		string _originaloptionset	=	null;
		string _fromoptionset		=	null;
		//bool _verbose				=	false;

		[TaskAttribute("OriginalOptionset", Required=true)]
		public string OriginalOptionset
		{
			get { return _originaloptionset; }
			set { _originaloptionset = value; }
		}

		[TaskAttribute("FromOptionset", Required=true)]
		public string FromOptionset
		{
			get { return _fromoptionset; }
			set { _fromoptionset = value; }
		}

		/// <summary>Execute the task.</summary>
		protected override void ExecuteTask()
		{
			OptionSet fromOS = Project.NamedOptionSets[_fromoptionset];
			OptionSet originalOS = Project.NamedOptionSets[_originaloptionset];

            if (fromOS == null)
            {
                string errorMessage = String.Format("ERROR:  OptionSet '{0}' specified as input to MergeOptionset task does not exist.", _fromoptionset);
                throw new BuildException(errorMessage);
            }

            if (originalOS == null)
            {
                string errorMessage = String.Format("ERROR:  OptionSet '{0}' specified for as output for the MergeOptionset task does not exist.", _originaloptionset);
                throw new BuildException(errorMessage);
            }

			foreach (DictionaryEntry entry in fromOS.Options)
			{
				string name  = (string) entry.Key;
				MergeOptionTask.MergeOptionValue( originalOS, name, entry.Value.ToString() ) ;
			}
		}

		public override bool ProbingSupported 
		{
			get { return true; }
		}
	}


	[TaskName("MergeOption")]
	public class MergeOptionTask : Task
	{
		string _optionset		=	null;
		string _optionName		=	null;
		string _optionValue     =   null ;

		[TaskAttribute("Optionset", Required=true)]
		public string Optionset
		{
			get { return _optionset; }
			set { _optionset = value; }
		}

		[TaskAttribute("OptionName", Required=true)]
		public string OptionName
		{
			get { return _optionName; }
			set { _optionName = value; }
		}

		//[Property("OptionValue", Required=true)]
		public string OptionValue
		{
			get { return _optionValue; }
			set { _optionValue = value; }
		}

		protected override void InitializeTask( XmlNode taskNode )
		{
			XmlElement valueElement = (XmlElement) taskNode.SelectSingleNode("OptionValue");
			if ( valueElement == null )
			{
				throw new BuildException( String.Format("Missing required <OptionValue> element.") );
			}
			OptionValue = Project.ExpandProperties( valueElement.InnerText ) ;
		}

		protected override void ExecuteTask()
		{
			OptionSet optionset = Project.NamedOptionSets[Optionset];
			if( null != optionset )
			{
				MergeOptionValue( optionset, OptionName, OptionValue ) ;
			}
		}

		static public void MergeOptionValue( OptionSet optionset, string name, string val )
		{
			string origVal ;
			origVal = optionset.Options[name] ;
			if( null != origVal && origVal.Length > 0 )
			{
				// replicate original MergeOption's behaviour, merge the option
				// only if it does not already exist. it is not entirely correct as 
				// the value may be part of substring, as in:
				//   "include/foo" "include/foobar"
				// employ simple substring approach for now...
				if( origVal.IndexOf( val ) >= 0 )
				{
					// do nothing: we already have it
					return  ;
				}

				// append new value to the end, along with separator
				val = origVal + " " + val ;
			}

			optionset.Options[name] = val ;

		}

		public override bool ProbingSupported 
		{
			get { return true; }
		}
	}

}
