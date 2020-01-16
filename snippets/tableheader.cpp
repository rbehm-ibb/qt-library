	QHeaderView *header = horizontalHeader();
	for (int i = 0; i < header->count(); ++i)
	{
		header->setSectionResizeMode(i, QHeaderView::ResizeToContents);
	}
	header->setStretchLastSection(true);
