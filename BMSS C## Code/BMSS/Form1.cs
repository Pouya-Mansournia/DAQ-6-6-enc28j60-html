using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace BMSS
{
    public partial class Form1 : Form
    {
        Relay Rel=null;
        public Form1()
        {
            InitializeComponent();
        }

        private void txtVolt1_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (Rel != null)
                {
                    Rel.SetActionStruct(0, int.Parse(txtVolt1.Text));
                }
            }
            catch { }
        }

        private void txtVolt2_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (Rel != null)
                {
                    Rel.SetActionStruct(1, int.Parse(txtVolt2.Text));
                }
            }
            catch { }
        }

        private void txtVolt3_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (Rel != null)
                {
                    Rel.SetActionStruct(2, int.Parse(txtVolt3.Text));
                }
            }
            catch { }
        }

        private void txtVolt4_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (Rel != null)
                {
                    Rel.SetActionStruct(3, int.Parse(txtVolt4.Text));
                }
            }
            catch { }
        }

        private void txtVolt5_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (Rel != null)
                {
                    Rel.SetActionStruct(4, int.Parse(txtVolt5.Text));
                }
            }
            catch { }
        }

        private void txtVolt6_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (Rel != null)
                {
                    Rel.SetActionStruct(5, int.Parse(txtVolt6.Text));
                }
            }
            catch { }
        }

        private void txtAccState_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (Rel != null)
                {
                    Rel.ChangeAcctionState(int.Parse(txtAccState.Text));
                }
            }
            catch { }
        }

        private void txtRFR_TextChanged(object sender, EventArgs e)
        {
            try
            {
                if (Rel != null)
                {
                    Rel.changeRefresh(int.Parse(txtRFR.Text));
                }
            }
            catch { }
        }

        private void ChCtrlAcc_CheckedChanged(object sender, EventArgs e)
        {
            if(Rel!=null)
            {
                Rel.CtrlActions(ChCtrlAcc.Checked);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                if (Rel == null )
                {
                    Rel = new Relay(this, StrIp.Text, int.Parse(txtRFR.Text), true, ChCtrlAcc.Checked);
                    button1.Text = "Stop";
                    return;
                }
                if (button1.Text.Equals("Stop"))
                {
                    Rel.Stop();
                    button1.Text = "Start";
                }
                else
                {
                    Rel.Start();
                    button1.Text = "Stop";
                }
            }
            catch { }
        }

        private void btnRel1_Click(object sender, EventArgs e)
        {
            if (Rel != null)
            {
                Rel.SetRelay("B1");
            }
        }

        private void btnRel2_Click(object sender, EventArgs e)
        {
            if (Rel != null)
            {
                Rel.SetRelay("B2");
            }
        }

        private void btnRel3_Click(object sender, EventArgs e)
        {
            if (Rel != null)
            {
                Rel.SetRelay("B3");
            }
        }

        private void btnRel4_Click(object sender, EventArgs e)
        {
            if (Rel != null)
            {
                Rel.SetRelay("B4");
            }
        }

        private void btnRel5_Click(object sender, EventArgs e)
        {
            if (Rel != null)
            {
                Rel.SetRelay("B5");
            }
        }

        private void btnRel6_Click(object sender, EventArgs e)
        {
            if (Rel != null)
            {
                Rel.SetRelay("B6");
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
