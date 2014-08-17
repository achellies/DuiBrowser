#ifndef _MANAGE_URLS_H
#define _MANAGE_URLS_H

class UrlInputBox;

void InitManageUrls();

void ReadHistoryUrls(UrlInputBox *urlInputBox);
void SaveHistoryUrl(LPCWSTR url, int nLen);

void ReadFavoriteUrls(HMENU hMenuTools);
void AddFavorite(HMENU hMenuTools);
void ManageFavorites(HMENU hMenuTools);

bool GetFavoriteUrl(UINT nCmdId, LPWSTR wszUrl, UINT &nLen);

#endif // _MANAGE_URLS_H
