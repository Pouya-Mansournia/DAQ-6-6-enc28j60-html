using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;

namespace BMSS
{
    public struct actionStruc
    {
        public string port;
        public int value;
        public bool state;
    };
    public class Relay
    {
        public Form1 frm= null;
        public Relay(Form1 f,string ip,int refresh=50,bool autostart=false,bool actEnabel=false)
        {
            this.RelQueue = new Queue<string>();
            this.IP = ip;
            this.RefreshRate = refresh;
            this.LiveTimer = new System.Windows.Forms.Timer();
            this.LiveTimer.Interval = RefreshRate;
            this.LiveTimer.Tick += new System.EventHandler(this.Dowork);
            this.LiveTimer.Enabled = false;
            this.ActionEnabeld = actEnabel;
            this.frm=f;

            this.SensValues = new int[6] { 0, 0, 0, 0, 0, 0 };
            this.Actions = new actionStruc[6];
            for (int i = 0; i <= 5;i++ )
            {
                actionStruc tmp = new actionStruc();
                tmp.port = "B" + (i + 1).ToString();
                tmp.value = 0;
                tmp.state = false;
            }
            if (autostart)
               this.Start();           
        }

        ~Relay()
        {
            this.LiveTimer.Dispose();
        }
        /////////////////////////////////////////////////////////////////////////////////////////
        public string IP;
        public int RefreshRate;
        public int SensVolt;
        public new int[] SensValues;
        public Queue<string> RelQueue;
        public System.Windows.Forms.Timer LiveTimer;
        public int ActionState;
        public actionStruc[] Actions;
        public bool ActionEnabeld;
        public DateTime LastDT = DateTime.Now;

        //////////////////////////////////////////////////////////////////////////////////////////////////////

        private void GetSensors()
        {
            throw new System.NotImplementedException();
        }

        public void SetRelay(string port)
        {
            this.RelQueue.Enqueue(port);
        }

        private void Dowork(object sender, EventArgs e)
        {
            string url= string.Empty;
            if (RelQueue.Count > 0)
            {
                string port = RelQueue.Dequeue();
                url = "http://" + this.IP + "/index.html?" + port;
                this.ChangeRelState(port);
            }
            else
            {
                url = "http://" + this.IP + "/index.html" ;
            }
            try
            {
                HttpWebRequest myRequest = (HttpWebRequest)WebRequest.Create(url);
                myRequest.Method = "GET";
                WebResponse myResponse = myRequest.GetResponse();
                StreamReader sr = new StreamReader(myResponse.GetResponseStream(), System.Text.Encoding.UTF8);
                string result = sr.ReadToEnd();
                if (result.Length > 5)
                   this.ReadResponse(result);
                sr.Close();
                myResponse.Close();
                frm.listBox1.Items.Add("Replay ... ");
            }
            catch { frm.listBox1.Items.Add("Time Out ..."); }
        }

        public void Start()
        {
            this.LiveTimer.Enabled = true;
        }

        public void Stop()
        {
            this.LiveTimer.Enabled = false;
        }

        private void ReadResponse(string result)
        {
           for (int i=0;i<=5;i++)
           {
               string sens = "ADC" + (i+1).ToString();
               int index = result.IndexOf(sens);
               string temp = result.Remove(0, index);
               int len = temp.IndexOf("</p>");               
               string volt = result.Substring(index + 5, len - 5);
               this.SensValues[i] = int.Parse(volt);
               if (this.ActionEnabeld)
               {
                   switch (this.ActionState)
                   {
                       case 0: if (this.SensValues[i] == this.Actions[i].value)
                                   {
                                       if (!this.Actions[i].state)
                                           this.SetRelay("B" + (i + 1).ToString());
                                   }
                           else
                               this.Actions[i].state = false;
                               break; // be equel
                       case 1: if (this.SensValues[i] < this.Actions[i].value)
                                   {
                                       if (!this.Actions[i].state)
                                           this.SetRelay("B" + (i + 1).ToString());
                                   }
                               else
                                   this.Actions[i].state = false;
                               break; // less than
                       case 2: if (this.SensValues[i] > this.Actions[i].value)
                                   {
                                       if (!this.Actions[i].state)
                                           this.SetRelay("B" + (i + 1).ToString());
                                   }
                               else
                                   this.Actions[i].state = false;
                               break; // greate than
                   }
               }
               switch (i)
               {
                   case 0: frm.lblVolt1.Text = volt; break;
                   case 1: frm.lblVolt2.Text = volt; break;
                   case 2: frm.lblVolt3.Text = volt; break;
                   case 3: frm.lblVolt4.Text = volt; break;
                   case 4: frm.lblVolt5.Text = volt; break;
                   case 5: frm.lblVolt6.Text = volt; break;
               }
           }
            frm.ticks.Text = (DateTime.Now - LastDT).TotalMilliseconds.ToString();
            LastDT = DateTime.Now;
        }
        public void SetActionStruct(int index , int val)
        {
            this.Actions[index].value = val;
        }

        public void CtrlActions(bool act)
        {
            this.ActionEnabeld = act;
        }
        public void changeRefresh(int reff)
        {
            this.RefreshRate = reff;
            this.LiveTimer.Interval = this.RefreshRate;
        }
        public void ChangeAcctionState(int ss)
        {
            this.ActionState = ss;
        }
        private void ChangeRelState(string port)
        {
            int index = int.Parse(port.Remove(0, 1));
            this.Actions[index - 1].state = !this.Actions[index-1].state;
           
            switch(index)
            {
                case 1:if(frm.btnRel1.Text.Equals("Off")) 
                            { frm.btnRel1.Text="On"; frm.btnRel1.BackColor=System.Drawing.Color.Green;}
                       else
                            { frm.btnRel1.Text="Off"; frm.btnRel1.BackColor=System.Drawing.Color.Red;}
                       break;
                case 2: if (frm.btnRel2.Text.Equals("Off"))
                       { frm.btnRel2.Text = "On"; frm.btnRel2.BackColor = System.Drawing.Color.Green; }
                       else
                       { frm.btnRel2.Text = "Off"; frm.btnRel2.BackColor = System.Drawing.Color.Red; }
                       break;
                case 3: if (frm.btnRel3.Text.Equals("Off"))
                       { frm.btnRel3.Text = "On"; frm.btnRel3.BackColor = System.Drawing.Color.Green; }
                       else
                       { frm.btnRel3.Text = "Off"; frm.btnRel3.BackColor = System.Drawing.Color.Red; }
                       break;
                case 4: if (frm.btnRel4.Text.Equals("Off"))
                       { frm.btnRel4.Text = "On"; frm.btnRel4.BackColor = System.Drawing.Color.Green; }
                       else
                       { frm.btnRel4.Text = "Off"; frm.btnRel4.BackColor = System.Drawing.Color.Red; }
                       break;
                case 5: if (frm.btnRel5.Text.Equals("Off"))
                       { frm.btnRel5.Text = "On"; frm.btnRel5.BackColor = System.Drawing.Color.Green; }
                       else
                       { frm.btnRel5.Text = "Off"; frm.btnRel5.BackColor = System.Drawing.Color.Red; }
                       break;
                case 6: if (frm.btnRel6.Text.Equals("Off"))
                       { frm.btnRel6.Text = "On"; frm.btnRel6.BackColor = System.Drawing.Color.Green; }
                       else
                       { frm.btnRel6.Text = "Off"; frm.btnRel6.BackColor = System.Drawing.Color.Red; }
                       break;
            }
        }
    }
}
