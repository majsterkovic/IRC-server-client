package irc.client;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellRenderer;
import java.awt.*;

/**
 * A class that represents a custom cell renderer for the JTable.
 * It is used to make the cells wrap the text.
 */
public class MyCellRender extends JTextArea implements TableCellRenderer {
    DefaultTableCellRenderer renderer;

    /**
     * Creates a new cell renderer.
     * @param table The table that the renderer is used for.
     */
    public MyCellRender(JTable table) {
        setLineWrap(true);
        setWrapStyleWord(true);
        renderer = (DefaultTableCellRenderer) table.getDefaultRenderer(Object.class);
        setFont(new Font("Consolas", Font.PLAIN, 14));
    }

    /**
     * Returns the component used for drawing the cell.
     * @param table The JTable that is asking the renderer to draw.
     * @param value The value of the cell to be rendered.
     * @param isSelected True if the cell is to be rendered with the selection highlighted.
     * @param hasFocus True if the cell has the focus.
     * @param row The row index of the cell being drawn.
     * @param column The column index of the cell being drawn.
     * @return The component used for drawing the cell.
     */
    public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        setText((String) value);
        setSize(table.getColumnModel().getColumn(column).getWidth(), getPreferredSize().height);
        if (table.getRowHeight(row) != getPreferredSize().height) {
            table.setRowHeight(row, getPreferredSize().height);
        }
        return this;
    }
}