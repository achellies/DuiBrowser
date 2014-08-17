//
// WebkitLayout.cpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2011 achellies (achellies at 163 dot com)
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#include "config.h"

#include <EAWebKit/EAWebKit.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Api/WebView.h>
#include <Api/WebPreferences.h>

#include <windows.h>
#include <windowsx.h>

#include <string>
#include <vector>

#include <EAWebKit/WebkitLayout.h>

#include "RenderText.h"
#include "InlineTextBox.h"
#include "InlineBox.h"
#include "FrameView.h"
#include "Page.h"
#include "SelectionController.h"
#include "Cache.h"
#include "Settings.h"
#include "Editor.h"
#include "TextIterator.h"
#include "FrameTree.h"
#include "RenderPart.h"
#include "RenderBlock.h"
#include "FrameLoader.h"

#include "iReader.h"

using namespace WebCore;

namespace WebCore
{
	class InlineTextBox;
	class StringImpl;

	inline RenderText* toRenderText(RenderObject* object)
	{ 
		ASSERT(!object || object->isText());
		return reinterpret_cast<RenderText*>(object);
	}

	inline const RenderText* toRenderText(const RenderObject* object)
	{ 
		ASSERT(!object || object->isText());
		return reinterpret_cast<const RenderText*>(object);
	}

	// This will catch anyone doing an unnecessary cast.
	void toRenderText(const RenderText*);
}

typedef CMemroySharePtr<iReaderData, 1> IReaderDataPtr;

namespace Hanvon
{
	namespace WebkitLayout
	{
		class WebkitLayoutPrivate
		{
		public:
			WebkitLayoutPrivate()
				: m_nCurPage(0)
			{}

			bool pageAvailable() const {
				return (!m_pagesRect.isEmpty() && (m_nCurPage >= 0) && (m_nCurPage <= (static_cast<int>(m_pagesRect.size()) - 1)));
			}

			int curPage() const {
				return m_nCurPage;
			}

			void setCurPage(int nPage) {
				m_nCurPage = nPage;
			}

			void reset() {
				m_pagesRect.clear();
				m_pagesOffset.clear();
				m_nCurPage = 0;
				m_nMaximumText = 0;
			}

			void setMaximumText(int maximumText) {
				m_nMaximumText = maximumText;
			}

			int maximumText() {
				return m_nMaximumText;
			}

			void append(IntRect rect) {
				m_pagesRect.append(rect);
			}

			void addTextOffset(int offset) {
				m_pagesOffset.append(offset);
			}

			int getTextOffset(int nIndex) {
				if (pageAvailable() && (!m_pagesOffset.isEmpty() && (nIndex >= 0) && (nIndex <= (static_cast<int>(m_pagesOffset.size()) - 1))))
					return m_pagesOffset.at(nIndex);

				return 0;
			}

			int curPageOffset() {
				return getTextOffset(m_nCurPage);
			}

			int getPageByOffset(int offset) {
				int nPage = 0;
				if (pageAvailable())
					for (Vector<int>::const_iterator citer = m_pagesOffset.begin();
						citer != m_pagesOffset.end(); ++citer, ++nPage)
						if (offset < *citer) {
							nPage = (nPage > 0)?(nPage - 1):0;
							break;
						}
						else if (offset == *citer)
							break;

				return nPage;
			}

			IntRect lastPageRect() const {
				IntRect pageRect;
				if (pageAvailable())
					pageRect = m_pagesRect.last();
				return pageRect;
			}

			IntRect firstPageRect() const {
				IntRect pageRect;
				if (pageAvailable())
					pageRect = m_pagesRect.first();
				return pageRect;
			}

			IntRect curPageRect() const {
				IntRect pageRect;
				if (pageAvailable())
					pageRect = m_pagesRect.at(m_nCurPage);

				return pageRect;
			}

			IntRect pageRect(int nIndex) const {
				IntRect pageRect;
				if (pageAvailable())
					pageRect = m_pagesRect.at(nIndex);

				return pageRect;
			}

			int pagesCount() const {
				return static_cast<int>(m_pagesRect.size());
			}
		private:
			// the current page index, start by 0
			int m_nCurPage;

			// the maximum text number for each page
			int m_nMaximumText;

			// the vector contains the pages rect information
			Vector<IntRect> m_pagesRect;

			// this vector contains the offset of each page
			Vector<int> m_pagesOffset;
		};

		class WebKitLayoutConcrete : public IWebKitLayout
		{
		public:
			WebKitLayoutConcrete()
			{
				m_iReaderData.Initalize(IREADER_DATA);
			}

			virtual ~WebKitLayoutConcrete()
			{}

		public:
			virtual void Load(LPCTSTR pURI, View* pView)
			{
				if (pView && pView->GetWebView())
				{
					::WebView* pWebView = pView->GetWebView();
					::WebFrame* pMainWebFrame = pView->GetWebFrame();
					double timeoutSeconds = (double)EA::WebKit::GetParameters().mPageTimeoutSeconds;

					const int cDefaultCacheCapacity = 5 * 1024 * 1024;
					WebCore::Cache* pCache = WebCore::cache();
					if (pCache)
					{
						pCache->setCapacities(0, cDefaultCacheCapacity, cDefaultCacheCapacity);
						pCache->setDisabled(true);
					}

					if (pMainWebFrame && pMainWebFrame->impl()->page())
					{
						Settings* settings = pMainWebFrame->impl()->page()->settings();

						if (settings)
						{
							//settings->setMaximumDecodedImageSize(static_cast<size_t>(cDefaultCacheCapacity / 5));
							//settings->setMediaEnabled(false);
							settings->setJavaEnabled(false);
							settings->setJavaScriptEnabled(false);
							settings->setPluginsEnabled(false);
							//settings->setLocalStorageEnabled(false);
							settings->setUsesPageCache(false);
						}
					}

					pMainWebFrame->loadURL(pURI, timeoutSeconds);
				}
			}

			virtual int BeginWebkitLayout(View* pView, RECT rcClient)
			{
				int nTotalPage = 0;

				if (!pView->GetWebFrame()->impl()->view()->didFirstLayout())
					return nTotalPage;
				
				WebCore::IntSize pageSize(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
				if (!pView->GetWebFrame()->impl()->view() || !pView->GetWebFrame()->impl() || pageSize.isEmpty())
					return nTotalPage;

				m_data.reset();

				pView->GetWebFrame()->impl()->view()->setScrollbarsMode(ScrollbarAlwaysOff);

				int contentsWidth = pView->GetWebFrame()->impl()->view()->contentsWidth();
				int contentsHeight = pView->GetWebFrame()->impl()->view()->contentsHeight();

				int visibleWidth = pView->GetWebFrame()->impl()->view()->visibleWidth();
				int visibleHeight = pView->GetWebFrame()->impl()->view()->visibleHeight();

				//_tprintf(_T("WebView::dividePages, contentsWidth = %d, contentsHeight = %d, visibleWidth = %d, visibleHeight = %d\n"),
				//	contentsWidth, contentsHeight, visibleWidth, visibleHeight);

				//_tprintf(_T("WebView::dividePages, rcClient.left = %d, rcClient.top = %d, rcClient.right = %d, rcClient.bottom=%d\n"),
				//	rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);

				int nPageCount = static_cast<int>(contentsHeight / pageSize.height()) + ((contentsHeight % pageSize.height())?1:0);

				Document* doc = pView->GetWebFrame()->impl()->document();
				if (doc && doc->firstChild()) {
					Vector<WebCore::IntRect> leafNodesRect;
					WalkThroughRecrusionNode(leafNodesRect, doc->firstChild());
					if (!leafNodesRect.isEmpty()) {
						WebCore::IntRect pageRect(0, 0, pageSize.width(), pageSize.height());
						for (int i = 0; i < (nPageCount + 3); ++i) {
							int nHeight = 0;
							if (m_data.pageAvailable()) {
								pageRect.setY(m_data.lastPageRect().y() + m_data.lastPageRect().height());
								pageRect.setHeight(pageSize.height());
								nHeight = pageRect.height();
							} else {
								nHeight = pageSize.height();
							}

							bool bHasLeafNode = false;				
							Vector<WebCore::IntRect> intersectsRect;
							for (Vector<WebCore::IntRect>::const_iterator citer = leafNodesRect.begin(); citer != leafNodesRect.end(); ++citer) {
								if (pageRect.contains(*citer)) {
									bHasLeafNode = true;
									if (nHeight < citer->bottom())
										nHeight = citer->bottom();
								} else {
									if (pageRect.intersects(*citer))
										intersectsRect.append(*citer);
								}
							}

							if (!bHasLeafNode)
							{
								nHeight = pageRect.y() + pageSize.height();
								for (Vector<WebCore::IntRect>::const_iterator citer = leafNodesRect.begin(); citer != leafNodesRect.end(); ++citer) {
									if (citer->contains(pageRect) || citer->intersects(pageRect)) {
										bHasLeafNode = true;
										break;
									}
								}
							}

							if (!bHasLeafNode)
								break;

							int yPos = pageRect.y() + pageRect.height();
							if (intersectsRect.isEmpty()) {
								pageRect.setHeight(nHeight - pageRect.y());
							} else {
								for (Vector<WebCore::IntRect>::const_iterator citer = intersectsRect.begin(); citer != intersectsRect.end(); ++citer) {
									if ((yPos > citer->y()) && (yPos < (citer->y() + citer->height())))
										yPos = citer->y();
								}

								if (yPos <= pageRect.y()) {
									pageRect.setY(m_data.lastPageRect().y() + m_data.lastPageRect().height());
								} else {
									pageRect.setHeight(yPos - pageRect.y());
								}
							}

							m_data.append(pageRect);
						}
					}
				}

				//_tprintf(_T("WebView::dividePages, pageCount = %d.\n"), m_data.pagesCount());
				if (m_data.pagesCount() > 0) {

					String text;
					int nOffset = 0;
					int nMaximumText = 0;
					m_data.addTextOffset(nOffset);
					for (int i = 0; i < m_data.pagesCount(); ++i) {
						text = GetPageText(i, pView);
						nOffset += text.length();
						if (nMaximumText < text.length())
						{
							nMaximumText = text.length();
						}

						if (i != m_data.pagesCount() - 1)
							m_data.addTextOffset(nOffset);
					}
					if (m_data.pagesCount() == 1) {
						nMaximumText = GetPageText(0, pView).length();
					}
					m_data.setMaximumText(nMaximumText);

					if (m_iReaderData) {
						m_iReaderData->m_nPagesCount = m_data.pagesCount();
						m_iReaderData->m_nMaximumText = m_data.maximumText();
					}
				}

				return (nTotalPage = m_data.pagesCount());
			}


			virtual void EndWebkitLayout(View* pView)
			{
				m_data.reset();

				if (!pView->GetWebFrame()->impl()->view() || !pView->GetWebFrame()->impl())
					return;

				pView->GetWebFrame()->impl()->view()->setScrollbarsMode(ScrollbarAuto);
			}

			virtual int NavigatePage(int nIndex, View* pView)
			{
				int nPageHeight = 0;
				if (m_data.pageAvailable()) {

					m_data.setCurPage(nIndex);

					ClearTextSelection(pView);

					//pView->GetWebFrame()->impl()->view()->setContentsPos(m_data.curPageRect().x(), m_data.curPageRect().y());
					//pView->GetWebFrame()->impl()->view()->updateContents(m_data.curPageRect());
					pView->GetWebFrame()->impl()->view()->scrollRectIntoViewRecursively(m_data.curPageRect());
					//GraphicsContext context((PlatformGraphicsContext*)0);
					//context.setPaintingDisabled(true);
					//pView->GetWebFrame()->impl()->paint(&context, m_data.curPageRect());

					nPageHeight = m_data.curPageRect().height();

					if (m_iReaderData)
						m_iReaderData->m_nCurPage = nIndex;

					return nPageHeight;
				}

				return nPageHeight;
			}

			virtual void ClearCache(View* pView)
			{
				pView->CancelLoad();

				WebCore::Cache* pCache = WebCore::cache();
				pCache->setDisabled(true);
			}

			virtual int PageCount(View* pView)
			{
				return m_data.pageAvailable()?m_data.pagesCount():0;
			}

			virtual int CurPage(View* pView)
			{
				return m_data.pageAvailable()?m_data.curPage():0;
			}

			virtual int FirstPage(View* pView)
			{
				return NavigatePage(0, pView);
			}

			virtual int LastPage(View* pView)
			{
				return NavigatePage(m_data.pagesCount() - 1, pView);
			}

			virtual int NextPage(View* pView)
			{
				if (m_data.pageAvailable()) {

					int nCurPage = CurPage(pView);
					++nCurPage;
					if (nCurPage > m_data.pagesCount() - 1)
					{
						nCurPage = m_data.pagesCount() - 1;
					}

					return NavigatePage(nCurPage, pView);
				}

				return 0;
			}

			virtual int PrevPage(View* pView)
			{
				if (m_data.pageAvailable()) {

					int nCurPage = CurPage(pView);
					--nCurPage;
					if (nCurPage < 0)
					{
						nCurPage = 0;
					}

					return NavigatePage(nCurPage, pView);
				}

				return 0;
			}

			virtual int GetCurrOffset(View* pView)
			{
				if (m_data.pageAvailable())
					return m_data.curPageOffset();

				return 0;
			}

			virtual void FontSizeChanged(View* pView)
			{
				m_data.reset();

				ClearTextSelection(pView);

				Settings* settings = pView->GetWebFrame()->impl()->page()->settings();

				if (settings && m_iReaderData && (_tcslen(m_iReaderData->m_szFontFamily) > 0) && (m_iReaderData->m_nFontSize > 0)) {
					String fontFamily = settings->standardFontFamily();
					if (fontFamily.isEmpty() || (_tcsicmp(fontFamily.charactersWithNullTermination(), m_iReaderData->m_szFontFamily) != 0))
						settings->setStandardFontFamily(m_iReaderData->m_szFontFamily);

					settings->setDefaultFixedFontSize(m_iReaderData->m_nFontSize);
					settings->setDefaultFontSize(m_iReaderData->m_nFontSize);
					settings->setMinimumFontSize(m_iReaderData->m_nFontSize);
					settings->setMinimumLogicalFontSize(m_iReaderData->m_nFontSize);
				}

				if (pView->GetWebFrame()->impl()->view())
					pView->GetWebFrame()->impl()->view()->layout();
			}

			virtual int GetPageNumByOffset(int nOffset, View* pView)
			{
				if (m_data.pageAvailable())
					return m_data.getPageByOffset(nOffset);

				return 0;
			}

			virtual int GetPageOffset(int nIndex, View* pView)
			{
				if (m_data.pageAvailable())
					return m_data.getTextOffset(nIndex);

				return 0;
			}

			virtual LPWSTR GetSelectedText(View* pView)
			{
				String selectedText;
				if (pView->GetWebFrame()->impl() && pView->GetWebFrame()->impl()->view())
					selectedText = pView->GetWebFrame()->impl()->selectedText();

				if (!m_stringManager)
					m_stringManager.Initialize(m_data.maximumText());

				if (m_stringManager && !selectedText.isEmpty())
					m_stringManager.FillBuffer(selectedText.charactersWithNullTermination(), selectedText.length());

				if (!selectedText.isEmpty())
				{
					LPWSTR buffer = (LPWSTR)(void*)LocalAlloc(LPTR, 2 * (selectedText.length() + 1));
					memcpy_s(buffer, 2 * selectedText.length(), (void*)selectedText.charactersWithNullTermination(), 2 * selectedText.length());
					buffer[selectedText.length()] = '\0';
					return buffer;
				}

				return NULL;
			}

			virtual LPWSTR GetPageText(int nIndex, View* pView, bool bStoredIntoStringShare)
			{
				String text;

				if (!pView->GetWebFrame()->impl()->view() || !pView->GetWebFrame()->impl() || !m_data.pageAvailable()) {
					return NULL;
				}

				if (bStoredIntoStringShare && !m_stringManager)
					m_stringManager.Initialize(m_data.maximumText());

				Document* doc = pView->GetWebFrame()->impl()->document();
				if (doc && doc->firstChild()) {
					text = GetRecrusionNodeText(nIndex, doc->firstChild());
				}

				if (bStoredIntoStringShare && m_stringManager && !text.isEmpty())
					m_stringManager.FillBuffer(text.charactersWithNullTermination(), text.length());

				if (!text.isEmpty())
				{
					LPWSTR buffer = (LPWSTR)(void*)LocalAlloc(LPTR, 2 * (text.length() + 1));
					memcpy_s(buffer, 2 * text.length(), (void*)text.charactersWithNullTermination(), 2 * text.length());
					buffer[text.length()] = '\0';
					return buffer;
				}

				return NULL;
			}

			virtual bool SearchText(LPCTSTR pSearchingText, View* pView, unsigned limit, unsigned& matches, unsigned** ppMatchedPages)
			{
				if (pSearchingText && ppMatchedPages)
				{
					String target = pSearchingText;
					bool found = false;

					if (!pView->GetWebFrame()->impl()->view() || !pView->GetWebFrame()->impl() || !pView->GetWebFrame()->impl()->page())
						return false;

					std::vector<unsigned> matchedPages;
					matches = 0;

					unsigned limit = 0;
					TextCaseSensitivity caseSensitivity = TextCaseSensitive;
					bool shouldHighlight = true;

					bool caseFlag = caseSensitivity == TextCaseSensitive;
					limit = limit ? (limit - matches) : 0;
					bool markMatches = true;

					Frame* frame = pView->GetWebFrame()->impl()->page()->mainFrame();
					do {
						frame->setMarkedTextMatchesAreHighlighted(shouldHighlight);
						//matches += frame->markAllMatchesForText(target, caseSensitivity == TextCaseSensitive, (limit == 0) ? 0 : (limit - matches));

						RefPtr<Range> searchRange(rangeOfContents(frame->document()));

						ExceptionCode exception = 0;
						unsigned matchCount = 0;
						do {
							RefPtr<Range> resultRange(findPlainText(searchRange.get(), target, true, caseFlag));
							if (resultRange->collapsed(exception)) {
								if (!isInShadowTree(resultRange->startContainer()))
									break;

								searchRange = rangeOfContents(frame->document());
								searchRange->setStartAfter(resultRange->startContainer()->shadowAncestorNode(), exception);
								continue;
							}

							// A non-collapsed result range can in some funky whitespace cases still not
							// advance the range's start position (4509328). Break to avoid infinite loop.
							VisiblePosition newStart = endVisiblePosition(resultRange.get(), DOWNSTREAM);
							if (newStart == startVisiblePosition(searchRange.get(), DOWNSTREAM))
								break;

							++matchCount;

							frame->document()->addMarker(resultRange.get(), DocumentMarker::TextMatch);

							IntRect resultRect = resultRange->boundingBox();
							if (m_data.pageAvailable()) {
								for (int nIndex = 0; nIndex < m_data.pagesCount(); ++nIndex) {
									if (m_data.pageRect(nIndex).contains(resultRect)
										&& (find(matchedPages.begin(), matchedPages.end(), nIndex) == matchedPages.end())) {
											matchedPages.push_back(nIndex);
											break;
									}
								}
							}

							// Stop looking if we hit the specified limit. A limit of 0 means no limit.
							if (limit > 0 && matchCount >= limit)
								break;

							setStart(searchRange.get(), newStart);
							if (searchRange->collapsed(exception) && isInShadowTree(searchRange->startContainer())) {
								Node* shadowTreeRoot = searchRange->startContainer();
								while (shadowTreeRoot && !shadowTreeRoot->isShadowNode())
									shadowTreeRoot = shadowTreeRoot->parentNode();
								if (shadowTreeRoot)
									searchRange->setEnd(shadowTreeRoot, shadowTreeRoot->childNodeCount(), exception);
							}
						} while (true);

						// Do a "fake" paint in order to execute the code that computes the rendered rect for 
						// each text match.
						Document* doc = frame->document();
						if (doc && frame->contentRenderer()) {
							doc->updateLayout(); // Ensure layout is up to date.
							IntRect visibleRect(enclosingIntRect(frame->view()->visibleContentRect()));
							GraphicsContext context((PlatformGraphicsContext*)0);
							context.setPaintingDisabled(true);
							frame->paint(&context, visibleRect);
						}
						matches += matchCount;

						//frame = incrementFrame(frame, true, false);
						frame = frame->tree()->traverseNextWithWrap(false);
					} while (frame);

					if (matches > 0)
					{
						*ppMatchedPages = (unsigned*)LocalAlloc(LPTR, matches * sizeof(unsigned));
						if (*ppMatchedPages != NULL)
						{
							int i = 0;
							for (std::vector<unsigned>::const_iterator citer = matchedPages.begin(); citer != matchedPages.end(); ++citer, ++i)
							{
								(*ppMatchedPages)[i] = *citer;
							}
							return true;
						}
					}

					return false;
				}
				return false;
			}

			virtual void ClearTextSelection(View* pView)
			{
				if (pView->GetWebFrame()->impl()->page()) {
					pView->GetWebFrame()->impl()->page()->removeAllVisitedLinks();
					if (!m_iReaderData || !m_iReaderData->m_bUnderSearch)
						pView->GetWebFrame()->impl()->page()->unmarkAllTextMatches();
				}

				if (pView->GetWebFrame()->impl() && pView->GetWebFrame()->impl()->selection())
					pView->GetWebFrame()->impl()->selection()->clear();

				if (m_iReaderData)
					m_iReaderData->m_hasSelectionText = false;
			}

		private:

			bool isInShadowTree(Node* node)
			{
				for (Node* n = node; n; n = n->parentNode())
					if (n->isShadowNode())
						return true;
				return false;
			}

			String GetPageText(int nIndex, View* pView)
			{
				String text;

				if (!pView->GetWebFrame()->impl()->view() || !pView->GetWebFrame()->impl() || !m_data.pageAvailable()) {
					return text;
				}

				Document* doc = pView->GetWebFrame()->impl()->document();
				if (doc && doc->firstChild()) {
					text = GetRecrusionNodeText(nIndex, doc->firstChild());
				}

				return text;
			}

			String GetRecrusionNodeText(int nIndex, WebCore::Node* node)
			{
				String text;
				if (node) {
					for (; node; node = node->nextSibling()) {
						if (node->hasChildNodes())
							text += GetRecrusionNodeText(nIndex, node->firstChild());
						else if (node->renderer() && node->renderer()->isText())
							text += GetRecursionRenderObjectText(nIndex, toRenderText(node->renderer()));
					}
				}
				return text;
			}

			String GetRecursionRenderObjectText(int nIndex, WebCore::RenderText* pText)
			{
				String text;

				if (pText && pText->node()) {

					IntRect rect = pText->node()->getRect();
					rect = pText->absoluteContentBox();

					String strText = pText->text();
					for (InlineTextBox* pTextBox = pText->firstTextBox(); pTextBox; pTextBox = pTextBox->nextTextBox()) {
						IntRect textRect(pTextBox->xPos(), rect.y() + pTextBox->yPos(), pTextBox->width(), pTextBox->height());
						if (m_data.pageRect(nIndex).contains(textRect)) {
							unsigned start = pTextBox->start();
							unsigned end = pTextBox->end();
							unsigned len = pTextBox->len();
							text += strText.substring(start, len);
						}
					}

					for (RenderObject* child = pText->firstChild(); child && child->isText(); child = child->nextSibling())
						text += GetRecursionRenderObjectText(nIndex, toRenderText(child));
				}

				return text;
			}

			void WalkThroughRecrusionNode(Vector<WebCore::IntRect>& leafNodesRect, WebCore::Node* node)
			{
				if (node) {
					for (; node; node = node->nextSibling()) {
						if (node->hasChildNodes()) {
							if (node->renderer() && node->renderer()->isImage()) {
								WebCore::IntRect parentRect = node->getRect();
								leafNodesRect.append(parentRect);
							}
							else
								WalkThroughRecrusionNode(leafNodesRect, node->firstChild());
						}
						else
							GetLeafNodeRect(leafNodesRect, node);
					}
				}
			}

			void GetLeafNodeRect(Vector<WebCore::IntRect>& leafNodesRect, WebCore::Node* node)
			{
				if (node && node->renderer()) {
					WebCore::IntRect baseRect =  node->getRect();
					if (node->renderer())
						baseRect = node->renderer()->absoluteContentBox();

					if (node->renderer()->isText()
						|| node->renderer()->isTextArea()
						|| node->renderer()->isTextField()
						//|| node->renderer()->isRubyText()
#if defined(ENABLE_SVG) && ENABLE_SVG
						|| node->renderer()->isSVGText()
						|| node->renderer()->isSVGInlineText()
#endif
						)
						node->renderer()->absoluteRects(leafNodesRect, baseRect.x(), baseRect.y());
					else if (node->renderer()->isImage()
						//|| node->renderer()->isVideo()
						|| node->renderer()->isTable()
#if defined(ENABLE_SVG) && ENABLE_SVG
						|| node->renderer()->isSVGPath()
						|| node->renderer()->isSVGImage()
#endif
						)
						leafNodesRect.append(baseRect);
					else
						leafNodesRect.append(baseRect);
				}
				return;
			}

			String SelectNode(View* pView, const WebCore::IntPoint& clickedPoint, bool useRelativePostion = true)
			{
				String pressedText;
				if (!pView->GetWebFrame()->impl()->view() || !pView->GetWebFrame() || !pView->GetWebFrame()->impl()->document())
					return pressedText;

				WebCore::IntPoint absolutePoint = clickedPoint;
				IntRect pageRect;
				if (m_data.pageAvailable()) {
					pageRect = m_data.curPageRect();
					if (useRelativePostion) {
						absolutePoint.setX(pageRect.x() + clickedPoint.x());
						absolutePoint.setY(pageRect.y() + clickedPoint.y());
					}
				}
				//else {
				//	RECT rcClient = {0};
				//	IntSize scrollSize = pView->GetWebFrame()->impl()->view()->scrollOffset();
				//	if (useRelativePostion) {
				//		absolutePoint.setX(clickedPoint.x() + scrollSize.width());
				//		absolutePoint.setY(clickedPoint.y() + scrollSize.height());
				//	}

				//	GetClientRect(m_windowHandle, &rcClient);
				//	pageRect.setX(scrollSize.width());
				//	pageRect.setY(scrollSize.height());
				//	pageRect.setWidth(rcClient.right - rcClient.left);
				//	pageRect.setHeight(rcClient.bottom - rcClient.top);
				//}

				Document* doc = pView->GetWebFrame()->impl()->document();
				if (doc && doc->firstChild()) {
					WebCore::Node* mousePressedNode = WalkThroughRecrusionNode(absolutePoint, doc->firstChild());

					if (mousePressedNode && mousePressedNode->renderer()) {
						if (mousePressedNode->renderer()->isText()) {
							WebCore::RenderText* pTextObject = toRenderText(mousePressedNode->renderer());
							IntRect pressNodeRect = mousePressedNode->getRect();
							String nodeText = pTextObject->text();

							if (mousePressedNode->renderer())
								pressNodeRect = mousePressedNode->renderer()->absoluteContentBox();

							for (InlineTextBox* pTextBox = pTextObject->firstTextBox(); pTextBox; pTextBox = pTextBox->nextTextBox()) {
								IntRect textRect(pTextBox->xPos(), pressNodeRect.y() + pTextBox->yPos(), pTextBox->width(), pTextBox->height());
								if (textRect.contains(absolutePoint.x(), absolutePoint.y())) {
									unsigned start = pTextBox->start();
									unsigned end = pTextBox->end();
									unsigned len = pTextBox->len();
									if (absolutePoint.x() >= textRect.x()) {
										unsigned textWidth = static_cast<unsigned>(pTextBox->width() / len);
										unsigned textoffset = static_cast<unsigned>((absolutePoint.x() - textRect.x()) / textWidth);
										pressedText = nodeText.substring(start + textoffset, 1 + (((absolutePoint.x() - textRect.x()) % textWidth))?1:0);
									}
								}
							}
						}

						IntRect pressNodeRect = mousePressedNode->getRect();
						RenderObject* render = mousePressedNode->renderer();

						if (mousePressedNode->renderer())
							pressNodeRect = mousePressedNode->renderer()->absoluteContentBox();
					}
				}

				return pressedText;
			}

			WebCore::Node* WalkThroughRecrusionNode(const WebCore::IntPoint& absolutePoint, WebCore::Node* node)
			{
				if (node) {
					for (; node; node = node->nextSibling()) {
						if (node->hasChildNodes()) {
							WebCore::Node* leafNode = WalkThroughRecrusionNode(absolutePoint, node->firstChild());
							if (leafNode != NULL)
								return leafNode;
						} else {
							WebCore::Node* leafNode = SelectLeafNode(absolutePoint, node);
							if (leafNode != NULL)
								return leafNode;
						}
					}
				}
			}

			WebCore::Node* SelectLeafNode(const WebCore::IntPoint& absolutePoint, WebCore::Node* node)
			{
				if (node && node->renderer()) {
					IntRect pressNodeRect = node->getRect();
					if (node->renderer())
						pressNodeRect = node->renderer()->absoluteContentBox();

					if (node->renderer()->isText()
						|| node->renderer()->isTextArea()
						|| node->renderer()->isTextField()
						//|| node->renderer()->isRubyText()
#if defined(ENABLE_SVG) && ENABLE_SVG
						|| node->renderer()->isSVGText()
						|| node->renderer()->isSVGInlineText()
#endif
						) {
							Vector<IntRect> leafNodesRect;
							node->renderer()->absoluteRects(leafNodesRect, pressNodeRect.x(), pressNodeRect.y());
							for (Vector<IntRect>::const_iterator citer = leafNodesRect.begin(); citer != leafNodesRect.end(); ++citer)
							{
								if (citer->contains(absolutePoint))
									return node;
							}		
					}
					else if (node->renderer()->isImage()
						//|| node->renderer()->isVideo()
						|| node->renderer()->isTable()
#if defined(ENABLE_SVG) && ENABLE_SVG
						|| node->renderer()->isSVGPath()
						|| node->renderer()->isSVGImage()
#endif
						){
							if (pressNodeRect.contains(absolutePoint))
								return node;
					}
				}

				return NULL;
			}

		private:
			WebkitLayoutPrivate m_data;

			// 共享内存，这里不负责创建，只负责使用
			IReaderDataPtr m_iReaderData;
			CStrSharingManager m_stringManager;
		};
	}
}

#if (EAWEBKIT_PS3_PRX || EAWEBKIT_XENON_DLL || EAWEBKIT_WINDOWS_DLL || EAWEBKIT_WINCE_DLL)
extern "C" Hanvon::WebkitLayout::IWebKitLayout* CreateWebkitLayoutInstance(void)
{
	static Hanvon::WebkitLayout::WebKitLayoutConcrete concreteInstance;
	return &concreteInstance;
}
#endif