//-------------------------------------------------------------------------
// Terse File Format
// -----------------
// (c) 2022 Will Bickford
// MIT License
//-------------------------------------------------------------------------
// Introduction
// ------------
// Traditional text files are great. You can view an entire page of text in
// the space required to store the document (80x50 = 4KB). There's no extra
// formatting or markup. Text is the OG WYSIWYG format.
//
// Let's review what text files are from first-principles:
// * Text = a serialized (1-dimensional) stream of characters
// * Page = a 2-dimensional array of _lines_ of varying lengths
//
// What might text in higher dimensions look like? How would we make sense
// of it? Why does it even matter? These are questions you should ponder
// as you study what Terse Text can help you do.
//
// History Lesson
// --------------
// Early PCs (1976 - 1994) came with floppy disks that stored a very small
// amount of data (80 KB to 1,440 KB). Writing data to these drives was
// an exercise in patience: they were SLOW. Seek times of 200ms and data
// transfer rates of 100 KB/sec were common.
//
// Filling a 1.2 MB floppy disk at 100 KB/sec would take you 12 seconds!
//
// See: https://www.sciencedirect.com/topics/engineering/floppy-disk
//
// Let's put that into context for pages of text. Since CPUs were slow in
// those days, text was usually not compressed. So a floppy disk might be
// able to load 50 pages of text per second, once the drive head was moved
// to the correct sector. If those pages were scattered across the disk,
// then throughput drops to something more like 5 pages per second.
//
// Now, try convincing someone to work with higher-dimensional datasets
// when their disk subsystem can only transfer 5-50 pages of text per
// second. It would be pointless - a few page breaks might be all you need.
//
// And thus, Word Processors were born.
//
// The Internet Era
// ----------------
// In 1995, the Internet started to scale out very rapidly. At the same
// time, computers became fast enough for games, video, and 3D graphics.
// Very quickly, our systems went from being text machines to entertainment
// devices. Along the way, we forgot why our text abstractions even
// existed: we just took them for granted and did the best we could.
//
// The AGI Era
// -----------
// Terse Text is designed for 2040 and beyond. It is assumed that humanity
// will be using systems that routinely store petabytes of data and that
// we will have brain interfaces which allow us to interact with our
// computers in hyperspace. Computer screens will seem antiquated by then.
//
// OK, so why now? Well, because SSDs are stupidly fast.
//
// 2022 - 2040
// -----------
// A modern PCIe 5.0 SSD is FAST. For random 4KB writes, you can achieve
// transfer rates of 200 MB/sec. For large file sequential writes, the
// rate climbs to 10+ GB/sec. SSD Latency ("seek time") is 0.2 ms. This
// means that all point-in-time text-based datasets are now trivial to
// dump to disk. A 32-bit dataset is at most 4 GB, which can be saved
// to disk in about 400 ms - IF you don't have it scattered across a
// ton of files. Operating system overhead is now the I/O bottleneck.
//
// Going back to our floppy disk scenario...a modern SSD has the capacity
// of about 3 million floppy disks and the transfer rate of about
// 100,000 drives. CPUs are significantly faster as well, so compression
// on-the-fly is feasible - boosting effective transfer rates for text
// by 10X. A modern disk can thus write 100 GB/sec of compressed text.
//
// This gives us access to a text space of 50 million pages. If we break
// this text space up into manageable chunks, we will need a space that
// is 100 x 100 x 100 x 50 characters. This corresponds to the first
// 4 dimensions of Terse (Columns, Lines, Scrolls, and Sections).
//
// But Terse doesn't stop there. We intend for this format to be the
// text format for the AGI age. So we turned the dial up to 11. This
// should give us enough flexibility to manage very complex interactions
// with our computers.
//
// File Format Design
// ------------------
// Terse files are meant to be plain text extended to 11 dimensions.
//
// Terse repurposes some historic ASCII control codes (0x01 - 0x1F) to
// provide users with up to 11 dimensions of free-form text. All other
// characters not listed here are standard UTF-8 text.
//-------------------------------------------------------------------------
// Scroll => 3D, Section => 4D,  Chapter    => 5D
// Book   => 6D, Volume  => 7D,  Collection => 8D
// Series => 9D, Shelf   => 10D, Library    => 11D
//-------------------------------------------------------------------------
#define LIBRARY_BREAK       '\x01' // [Y] 11th dimension: historic: start of heading
#define BINARY_START        '\x02' // [Y] Binary dimension: historic: start of text
#define END_OF_TEXT         '\x03' //     ETX - backwards-compatible
#define END_OF_TRANSMISSION '\x04' //     EOT - backwards-compatible
#define COORDINATE_START    '\x05' // [Y] \q - historic: enquiry
#define COORDINATE_END      '\x06' // [Y] \k - historic: ack
#define MORE_COWBELL        '\x07' //     \a - historic: bell
#define BACKSPACE           '\x08' //     \b - historic: backspace
#define HORIZONTAL_TAB      '\x09' //     \t - historic: tab
#define LINE_BREAK          '\x0a' // [Y] Why mess with a good thing?
#define VERTICAL_TAB        '\x0b' //     \v - historic: Vertical Tab
#define FORM_FEED           '\x0c' //     \f - historic: Form Feed
#define CARRIAGE_RETURN     '\x0d' //     \r - historic: Carriage Return
#define START_COMMENT       '\x0e' // [Y] Comment Start;  historic: shift out
#define END_COMMENT         '\x0f' // [Y] Comment End;    historic: shift in
#define SCROLL_BREAK        '\x17' // [Y] 3rd dimension;  historic: end of transmission
#define SECTION_BREAK       '\x18' // [Y] 4th dimension;  historic: cancel
#define CHAPTER_BREAK       '\x19' // [Y] 5th dimension;  historic: end of medium
#define BOOK_BREAK          '\x1a' // [Y] 6th dimension;  historic: substitute
#define ESCAPE              '\x1b' //     \e - historic: Escape Key
#define VOLUME_BREAK        '\x1c' // [Y] 7th dimension;  historic: file separator
#define COLLECTION_BREAK    '\x1d' // [Y] 8th dimension;  historic: group separator
#define SERIES_BREAK        '\x1e' // [Y] 9th dimension;  historic: record separator
#define SHELF_BREAK         '\x1f' // [Y] 10th dimension; historic: unit separator